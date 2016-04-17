

#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>
#include "connection_manager.hpp"
#include "request_handler.hpp"
#include <boost/log/trivial.hpp>

namespace ftp {
	namespace server {

		connection::connection(boost::asio::io_service& io_service,
			connection_manager& manager, std::string strFtpDir)
			: socket_(io_service),
			connection_manager_(manager),
			request_handler_(strFtpDir),
			m_strMyWelcome("welcome to mars ftp server\r\n")
		{
		}

		boost::asio::ip::tcp::socket& connection::socket()
		{
			return socket_;
		}

		void connection::start()
		{
			//before start say welcome
			//	std::string strMyWelcome = "welcome to mars ftp server";
			boost::asio::async_write(socket_,
				boost::asio::buffer(m_strMyWelcome),
				boost::bind(&connection::handle_write_welcome, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));


			//
			if (socket_.is_open())
			{
				boost::asio::async_read_until(socket_, buf, "\n",
					boost::bind(&connection::handle_read_line, shared_from_this()));
			}


			//socket_.async_read_some(boost::asio::buffer(buffer_),
			//	boost::bind(&connection::handle_read, shared_from_this(),
			//	boost::asio::placeholders::error,
			//	boost::asio::placeholders::bytes_transferred));
		}

		void connection::stop()
		{
			socket_.close();
		}
		void connection::handle_read_line()
		{
			std::istream is(&buf);
			std::string s;
			getline(is, s);
			if (s.size() > 0)
			{
				//console.read_line(s);

				if (socket_.is_open())
				{
					boost::asio::async_read_until(socket_, buf, "\n",
						boost::bind(&connection::handle_read_line, shared_from_this()));
				}
			}
		}

		void connection::handle_read(const boost::system::error_code& e,
			std::size_t bytes_transferred)
		{
			if (!e)
			{
				boost::tribool result;
				boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
					request_, buffer_.data(), buffer_.data() + bytes_transferred);

				if (result)
				{
					request_handler_.handle_request(request_, reply_);
					boost::asio::async_write(socket_, reply_.to_buffers(),
						boost::bind(&connection::handle_write, shared_from_this(),
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
				}
				else if (!result)
				{
					reply_ = reply::stock_reply(reply::bad_request);
					boost::asio::async_write(socket_, reply_.to_buffers(),
						boost::bind(&connection::handle_write, shared_from_this(),
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
				}
				else
				{
					socket_.async_read_some(boost::asio::buffer(buffer_),
						boost::bind(&connection::handle_read, shared_from_this(),
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
				}
			}
			else if (e != boost::asio::error::operation_aborted)
			{
				connection_manager_.stop(shared_from_this());
			}
		}

		void connection::handle_write(const boost::system::error_code& e, std::size_t bytes_transferred)
		{

			if (!e)
			{
				// Initiate graceful connection closure.
				boost::system::error_code ignored_ec;
				socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
			}

			if (e != boost::asio::error::operation_aborted)
			{
				connection_manager_.stop(shared_from_this());
			}
		}
		void connection::handle_write_welcome(const boost::system::error_code& e, std::size_t bytes_transferred)
		{
			//error
			if (e)
			{
				BOOST_LOG_TRIVIAL(error) << e.message()<<" "<< __FILE__ <<  __FUNCTION__ <<__LINE__;
				boost::system::error_code ignored_ec;
				socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
				connection_manager_.stop(shared_from_this());
				return;
			}
			if (!e)
			{
				//no error just return;
				return;
			}
		}

	} // namespace server
} // namespace http
