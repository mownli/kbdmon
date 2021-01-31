#ifndef LIBINPUT_WRAPPER_H
#define LIBINPUT_WRAPPER_H

#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <libinput.h>

#include <string>


class LibinputWrapper
{
public:
	LibinputWrapper() {};
	LibinputWrapper(const std::string& path_to_device);
	~LibinputWrapper() noexcept;

	enum class EventType {NONE, KEY_UP, KEY_DOWN};
	struct Event
	{
		EventType type = EventType::NONE;
		int scancode = -1;
	};

	Event* pollForEvent();
	Event* waitForEvent(int delay);
	void init(const std::string& path_to_device);
private:
	class EventWrapper
	{
		struct libinput** li;
	public:
		struct libinput_event* event_ll = nullptr;
		Event event{};

		Event* getEvent() noexcept;
		void destroyEvent();
		EventWrapper(struct libinput** li_) noexcept : li(li_) {};
	};

	const struct libinput_interface interface {
		.open_restricted {
			[](const char *path, int flags, void*) {
				int fd = open(path, flags);
				return fd < 0 ? -errno : fd;
			}
		},
		.close_restricted {
			[](int fd, void*) { close(fd); }
		}
	};

	struct libinput* li = nullptr;
	struct pollfd pfd{};

	EventWrapper evWrapper{&li};
};

#endif // LIBINPUT_WRAPPER_H
