#pragma once

#include <string>
#include <boost/noncopyable.hpp>

namespace ftp {
	namespace server {

		struct reply;
		struct request;
		class connection;

		/// The common handler for all incoming requests.
		class request_handler
			: private boost::noncopyable
		{
		public:
			/// Construct with a directory containing files to be served.
			explicit request_handler(const std::string& ftp_root);

			/// Handle a request and produce a reply.
			void handle_request(const std::string& req, connection *pConnection);

		private:
			/// The directory containing the files to be served.
			std::string m_strFtpRoot;

			/// Perform URL-decoding on a string. Returns false if the encoding was
			/// invalid.
		//	static bool url_decode(const std::string& in, std::string& out);
		public:
			std::string m_strUserName;
			std::string m_strPwd;
		};

	} // namespace server
} // namespace http

