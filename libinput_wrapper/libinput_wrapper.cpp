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
	if(libinput_dispatch(li))
		throw std::runtime_error("libinput_dispatch() error");
}

LibinputWrapper::~LibinputWrapper()
{
	if(evWrapper.isValid()) evWrapper.destroyEvent();
	if(li) libinput_unref(li);
}

std::optional<LibinputWrapper::Event> LibinputWrapper::waitForEvent(int delay)
{
	std::optional<Event> ret = {};
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
		Event* ptr = evWrapper.getEvent();
		if(ptr)
		{
			ret = {*ptr};
		}
	}
	return ret;
}
//std::optional<LibinputWrapper::Event> LibinputWrapper::pollForEvent()
//{
//	std::optional<LibinputWrapper::Event> ret = {};
//	for(bool stop = false; (stop == false) && evWrapper.pollForEvent(li);)
//	{
//		if(evWrapper.isValid())
//		{
//			DEBUG("VALID");
//			ret = {evWrapper.getEvent()};
//			stop = true;
//		}
//		else
//			ret = {};

//		evWrapper.destroyEvent();
//	}
//	libinput_dispatch(li);
//	return ret;
//}

//LibinputWrapper::Event LibinputWrapper::EventWrapper::getEvent()
//{
//	assert(isValid());

//	auto* kevent = libinput_event_get_keyboard_event(ptr);
//	auto key_state = libinput_event_keyboard_get_key_state(kevent);
//	if(key_state == LIBINPUT_KEY_STATE_RELEASED)
//		event.type = EventType::KEY_UP;
//	else
//		event.type = EventType::KEY_DOWN;
//	event.scancode = libinput_event_keyboard_get_key(kevent);

//	return event;
//}

LibinputWrapper::Event* LibinputWrapper::EventWrapper::getEvent()
{
	assert(*li);

	event.type = EventType::NONE;
	event.scancode = -1;

	struct libinput_event* event_ll = libinput_get_event(*li);

	if(event_ll)
	{
		valid = true;
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
		//libinput_dispatch(*li);
	}
	else
	{
		DEBUG("SHIT");
		valid = false;
		//libinput_dispatch(*li);

	}
	libinput_dispatch(*li);
	return &event;
}

//struct libinput_event* LibinputWrapper::EventWrapper::pollForEvent(struct libinput* li)
//{
//	assert(li != nullptr);

//	ptr = libinput_get_event(li);
//	if(ptr)
//	{
//		if(libinput_event_get_type(ptr) == LIBINPUT_EVENT_KEYBOARD_KEY)
//		{
//			valid = true;
//		}
//	}

//	return ptr;
//}

void LibinputWrapper::EventWrapper::destroyEvent()
{
	valid = false;
	libinput_event_destroy(ptr);
}
