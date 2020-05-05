#ifndef _MESSAGE_TYPES_H_
#define _MESSAGE_TYPES_H_

namespace Net13
{
	namespace Messages
	{
		enum EMessageType
		{
			MESSAGETYPE_UNRELIABLE = 0,
			MESSAGETYPE_RELIABLE = 1,
			MESSAGETYPE_SEQUENCED = 2,
		};

		enum EDisconnectReason
		{
			REASON_QUIT = 0,
			REASON_TIMED_OUT = 1,
			REASON_KICKED = 2,
			REASON_BANNED = 3,
		};

		enum EConnectionResult
		{
			RESULT_SUCCESS = 0,
			RESULT_TIMED_OUT = 1,
			RESULT_SERVER_FULL = 2,
			RESULT_BANNED = 3,
		};
	}
}

#endif