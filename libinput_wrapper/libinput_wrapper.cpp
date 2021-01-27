#include "libinput_wrapper.h"
#include <stdexcept>
#include <cassert>
#include <cstring>
#include "dbg.h"

LibinputWrapper::LibinputWrapper(const std::string& pathToDevice) :
	li(libinput_path_create_context(&interface, nullptr)),
	pfd({.fd = libinput_get_fd(li), .events = POLLIN})

{
	if(libinput_path_add_device(li, pathToDevice.c_str()) == nullptr)
		throw std::runtime_error("Failed to add device");

}

LibinputWrapper::~LibinputWrapper()
{
	evWrapper.destroyEvent();
	if(li) libinput_unref(li);
}

LibinputWrapper::Event* LibinputWrapper::waitForEvent(int delay)
{
	assert(li);
	Event* ret = nullptr;
	switch(poll(&pfd, 1, delay))
	{
	case -1:
		if(errno != EINTR)
		{
			const std::string header = "[ERROR] poll(): ";
			throw std::runtime_error(header + std::strerror(errno));
		}
		// Signal handling
		DEBUG("Caught a signal");
		break;
	case 0:
		// Timeout
		DEBUG("Timeout");
		break;
	default:
		DEBUG("Ready to read");
		libinput_dispatch(li);
		ret = evWrapper.getEvent();
	}
	return ret;
}

void LibinputWrapper::init(const std::string& pathToDevice)
{
	assert(!li);
	li = libinput_path_create_context(&interface, nullptr);
	pfd = {.fd = libinput_get_fd(li), .events = POLLIN};
	if(libinput_path_add_device(li, pathToDevice.c_str()) == nullptr)
		throw std::runtime_error("Failed to add device");
}

LibinputWrapper::Event* LibinputWrapper::pollForEvent()
{
	assert(li);
	if(libinput_dispatch(li))
		throw std::runtime_error("libinput_dispatch() error");
	return evWrapper.getEvent();
}

LibinputWrapper::Event* LibinputWrapper::EventWrapper::getEvent()
{
	assert(*li);
	if((event_ll = libinput_get_event(*li)))
	{
		if(libinput_event_get_type(event_ll) == LIBINPUT_EVENT_KEYBOARD_KEY)
		{
			auto* kevent = libinput_event_get_keyboard_event(event_ll);
			auto key_state = libinput_event_keyboard_get_key_state(kevent);

			if(key_state == LIBINPUT_KEY_STATE_RELEASED)
			{
				event.type = EventType::KEY_UP;
				DEBUG("Key released");
			}
			else
			{
				event.type = EventType::KEY_DOWN;
				DEBUG("Key pressed");
			}
			event.scancode = libinput_event_keyboard_get_key(kevent);
			DEBUG("LL scancode %d", event.scancode);
		}
		else
		{
			event.type = EventType::NONE;
			event.scancode = 0;
		}

		libinput_event_destroy(event_ll);
		event_ll = nullptr;
		libinput_dispatch(*li);
		return &event;
	}
	else
	{
		event.type = EventType::NONE;
		event.scancode = -1;
	}

	return nullptr;
}

void LibinputWrapper::EventWrapper::destroyEvent()
{
	if(event_ll)
	{
		libinput_event_destroy(event_ll);
		event_ll = nullptr;
	}
}
