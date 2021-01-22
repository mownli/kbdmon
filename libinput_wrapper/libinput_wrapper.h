#ifndef LIBINPUT_WRAPPER_H
#define LIBINPUT_WRAPPER_H

#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <libinput.h>

#include <string>
#include <optional>


class LibinputWrapper
{
public:
	LibinputWrapper(const std::string& pathToDevice);
	~LibinputWrapper();

	enum class EventType {KEY_UP, KEY_DOWN, NONE};
	struct Event
	{
		EventType type;
		int scancode;
	};

	Event* pollForEvent();
	Event* waitForEvent(int delay);
private:
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

	struct libinput* li;
	struct pollfd pfd;

	class EventWrapper
	{
		struct libinput** li;
	public:
		struct libinput_event* event_ll = nullptr;
		Event event;
		Event* getEvent();
		void destroyEvent();
		EventWrapper(struct libinput** li_) : li(li_) {};
	} evWrapper{&li};
};

#endif // LIBINPUT_WRAPPER_H
