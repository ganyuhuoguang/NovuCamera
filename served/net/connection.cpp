/*
 * Copyright (C) 2014 MediaSift Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <served/status.hpp>
#include <served/net/connection.hpp>
#include <served/net/connection_manager.hpp>
#include <served/request_error.hpp>
//#include "mainwindow.h"
#include <QString>

#include <utility>
#include <vector>

unsigned int IMAGEID = 0;
QString g_IMAGE_PATH_LIST[10] = {};
QString g_TRACKING_ID[10] = {"tracking ID: 002","tracking ID: 002","tracking ID: 002","tracking ID: 002","tracking ID: 002","tracking ID: 002","tracking ID: 002","tracking ID: 002","tracking ID: 002","tracking ID: 002"};
QString g_CONFSCORE[10] = {"conf score: ","conf score: ","conf score: ","conf score: ","conf score: ","conf score: ","conf score: ","conf score: ","conf score: ","conf score: "};

unsigned int g_BODY_IMAGEID = 0;
QString g_BODY_IMAGE_PATH_LIST[10]= {};
QString g_BODY_TRACKING_ID[10] = {"tracking ID: 002","tracking ID: 002","tracking ID: 002","tracking ID: 002","tracking ID: 002","tracking ID: 002","tracking ID: 002","tracking ID: 002","tracking ID: 002","tracking ID: 002"};
QString g_BODY_CONFSCORE[10] = {"conf score: ","conf score: ","conf score: ","conf score: ","conf score: ","conf score: ","conf score: ","conf score: ","conf score: ","conf score: "};
QString g_BODY_GENDER[10] = {"gender:male"};
QString g_BODY_AGE[10]= {"30"};

using namespace served;
using namespace served::net;

connection::connection( boost::asio::io_service &    io_service
                      , boost::asio::ip::tcp::socket socket
                      , connection_manager &         manager
                      , multiplexer        &         handler
                      , size_t                       max_req_size_bytes
                      , int                          read_timeout
                      , int                          write_timeout
                      )
	: _io_service(io_service)
	, _status(status_type::READING)
	, _socket(std::move(socket))
	, _connection_manager(manager)
	, _request_handler(handler)
	, _max_req_size_bytes(max_req_size_bytes)
	, _read_timeout(read_timeout)
	, _write_timeout(write_timeout)
	, _request()
	, _request_parser(_request, _max_req_size_bytes)
	, _read_timer(_io_service, boost::posix_time::milliseconds(read_timeout))
	, _write_timer(_io_service, boost::posix_time::milliseconds(write_timeout))
{}

void
connection::start()
{
	boost::system::error_code ec;
	boost::asio::ip::tcp::endpoint endpoint = _socket.remote_endpoint(ec);
	if (ec)
	{
		_connection_manager.stop(shared_from_this());
		return;
	}

	_request.set_source(endpoint.address().to_string());
	do_read();

	if ( _read_timeout > 0 )
	{
		auto self(shared_from_this());

		_read_timer.async_wait([this, self](const boost::system::error_code& error) {
			if ( error.value() != boost::system::errc::operation_canceled )
			{
				_connection_manager.stop(shared_from_this());
			}
		});
	}
}

void
connection::stop()
{
	_socket.close();
}

char *get_boundary(char *mfd)
{
    char *p, *end;
    char *boundary = NULL;
    int boundary_size;

    if ((p = strstr(mfd, "--")) == NULL) {
        return NULL;
    }

    p += 2;
    if ((end = strstr(p, "\r\n")) == NULL) {
        return NULL;
    }

    boundary_size = end - p + 1;
    boundary = (char *)malloc(boundary_size);

    strncpy(boundary, p, boundary_size);
    boundary[boundary_size - 1] = 0;

    return boundary;
}

char* mutipart_form_data(char *mfd, char *boundary, char **name, char **content)
{
    char *p, *end;
    int size;
    int boundary_size;

    *content = *name = NULL;
    if ( (p = strstr(mfd, "name=")) == NULL) {
        return (char *)-1;
    }
    p += 6;
    if ((end = strchr(p, '\"')) == NULL) {
        return (char *)-1;
    }

    // 获取name
        size = end - p + 1;
        *name = (char *)malloc(size);
        strncpy(*name, p, size);
        (*name)[size - 1] = 0;

        // 获取内容
        p = end + 1;
        if ((end = strstr(p, boundary)) == NULL) {
            free(*name);
            *name = NULL;
            return (char *)-1;
        }

        boundary_size = strlen(boundary);
        mfd = end + boundary_size;

        while(*p == '\r' || *p == '\n') {
            p++;
        }

        end -= 3;
        while(*end == '\r' || *end == '\n') {
            end--;
        }

        if (p > end) {
            free(*name);
            *name = NULL;
            return (char *)-1;
        }

        size = end - p + 2;
        *content = (char *)malloc(size);
        strncpy(*content, p, size);
        (*content)[size - 1] = 0;

        if (*(mfd + 1) == '-') {
            // 说明到了最后了
            return NULL;
        }

        return mfd;

}

void
connection::do_read()
{
	auto self(shared_from_this());

	_socket.async_read_some(boost::asio::buffer(_buffer.data(), _buffer.size()),
		[this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
			if (!ec)
			{
				request_parser_impl::status_type result;
				result = _request_parser.parse(_buffer.data(), bytes_transferred);

				if ( request_parser_impl::FINISHED == result )
				{
					// Parsing is finished, stop reading and send response.

                    //              std::cout << "enter HTTP_version:" << std::endl;
                    //				std::cout << _request.HTTP_version() << std::endl;
                    //				std::cout << "enter source:" << std::endl;
                    //				std::cout << _request.source() << std::endl;
                    //				std::cout << "enter body:" << std::endl;

                    //				std::cout << "enter method:" << std::endl;
                    //				std::cout << _request.method() << std::endl;

                                    //std::unordered_map<std::string, std::string>::iterator iter;
                                    std::cout << "enter header:" << std::endl;
                                    std::cout << _request.header("Content-Length") << std::endl;
                                    std::cout << _request.header("Content-Type") << std::endl;
                                    std::cout << _request.header("Content-Disposition") << std::endl;

                                    //get face ID and score
                                    char *boundary;
                                    char *mfd = (char *)_request.body().data();
                                    printf("_request.body = %s\n", _request.body().data());

                                    boundary = get_boundary((char *)_request.body().data());
                                    //printf("boundary=%s\n", boundary);
                                    bool face_flag = false;
                                    while (1) {
                                        char *name, *content;
                                        static int i =1;
                                        if(i == 1)
                                        {
                                            mfd = mutipart_form_data((char *)_request.body().data(), boundary, &name, &content);
                                        }
                                        else
                                        {
                                            mfd = mutipart_form_data(mfd, boundary, &name, &content);
                                        }
                                        i++;

                                        if (mfd == (char *)-1) {
                                            i=1;
                                            break;
                                        }
                                        printf("name = %s\n", name);
                                        printf("content = %s\n", content);
                                                // show
                                        if(!strcmp(name,"face-coordinates"))
                                        {
                                            face_flag = true;
                                            int count = 0;
                                            char *tmp = content;
                                            content += 10;
                                            strncpy(content, content, 3);
                                            printf("content1 = %s\n", content);
                                            while( *tmp != '\0')
                                            {
                                              if(*tmp++ == ',')
                                                 count++;
                                            }

                                            char delims[] = ",";
                                            char *result = NULL;
                                            int j = 0;
                                            result = strtok( content, delims );
                                            char PrintResult[40];

                                            while( result != NULL )
                                            {
                                                //printf( "result is \"%s %d\"\n", result,j );

                                                result = strtok( NULL, delims );
                                                j++;

                                                if(j==count-4)
                                                {
                                                    printf( " atof(result) is \" %f\"\n", atof(result) );
                                                    sprintf(PrintResult,"conf score: %.0f%%",atof(result)*100);
                                                    //printf( "PrintResult = %s\n", PrintResult);
                                                    g_CONFSCORE[(IMAGEID)%10]= PrintResult;
                                                }

                                                if(j==count-3)
                                                {
                                                    printf( " atof(result) is \" %d\"\n", atoi(result) );
                                                    sprintf(PrintResult,"tracking ID: %d%",atoi(result));
                                                    g_TRACKING_ID[(IMAGEID)%10] = PrintResult;
                                                }

                                            }

                                        }
                                        else if(!strcmp(name,"body-coordinates"))
                                        {
                                            face_flag = false;
                                            int count = 0;
                                            char *tmp = content;
                                            while( *tmp != '\0')
                                            {
                                              if(*tmp++ == ',')
                                                 count++;
                                            }

                                            char delims[] = ",";
                                            char *result = NULL;
                                            int j = 0;
                                            result = strtok( content, delims );
                                            char PrintResult[40];

                                            while( result != NULL )
                                            {
                                                //printf( "result is \"%s %d\"\n", result,j );

                                                result = strtok( NULL, delims );
                                                j++;

                                                if(j==count-1)
                                                {
                                                    //printf( " atof(result) is \" %f\"\n", atof(result) );
                                                    sprintf(PrintResult,"conf score: %.0f%%",atof(result)*100);
                                                    //printf( "PrintResult = %s\n", PrintResult);
                                                    g_BODY_CONFSCORE[(g_BODY_IMAGEID)%10]= PrintResult;
                                                }

                                                if(j==count)
                                                {
                                                    //printf( " atof(result) is \" %d\"\n", atoi(result) );
                                                    sprintf(PrintResult,"tracking ID: %d%",atoi(result));
                                                    g_BODY_TRACKING_ID[(g_BODY_IMAGEID)%10] = PrintResult;
                                                    //g_BODY_TRACKING_ID[10];
                                                    //g_BODY_CONFSCORE[10];
                                                    //g_BODY_GENDER[10];
                                                    //g_BODY_AGE[10];
                                                }
                                                std::cout << "enter result!" << std::endl;
                                            }
                                            std::cout << "enter while!" << std::endl;
                                        }
                                        else
                                        {
                                           // std::cout << "error json data!" << std::endl;
                                        }
                                         std::cout << "enter free!" << std::endl;
                    //                    free(name);
                    //                    free(content);
                                        if (mfd == NULL) {
                                            std::cout << "enter break!" << std::endl;
                                            break;

                                        }
                                        std::cout << "enter end!" << std::endl;
                                    }

                                        std::cout << "enter  pic data!" << std::endl;
                                        char *c = (char *)_request.body().data();
                                         std::cout << c << std::endl;
                                        char *p = strstr((char *)_request.body().data(), "jpeg");//before is .jpg
                                        std::cout << "error  data!" << std::endl;
                                        std::cout << p << std::endl;
                                        unsigned int size = atoi((const char*)_request.header("Content-Length").data());
                                        std::cout << size << std::endl;

                                        unsigned char firmware[size]={0};

                                        memcpy((char *)firmware,p + strlen(".jpg")+4,size);//   before is 5
                                        std::cout << "write  data!" << std::endl;
                                        std::cout << firmware << std::endl;
                                        FILE *fp;
                                        char PrintResult[40];
                                        if(face_flag == true)
                                        {
                                            sprintf(PrintResult,"./face_captures/face_%d.jpg",IMAGEID++);

                                            g_IMAGE_PATH_LIST[(IMAGEID-1)%10] = PrintResult;

                                            fp = fopen(PrintResult, "wb");
                                            fwrite(firmware, size, 1, fp);//163759
                                        }
                                        else
                                        {
                                            sprintf(PrintResult,"./body_captures/body_%d.jpg",g_BODY_IMAGEID++);
                                            g_BODY_IMAGE_PATH_LIST [(g_BODY_IMAGEID-1)%10] = PrintResult;
                                            fp = fopen(PrintResult, "wb");
                                            fwrite(firmware, size, 1, fp);//163759
                                        }

                                        fflush(fp);
                                        fclose(fp);





					_read_timer.cancel();
					_status = status_type::DONE;

					try
					{
						_request_handler.forward_to_handler(_response, _request);
					}
					catch (const served::request_error & e)
					{
						_response.set_status(e.get_status_code());
						_response.set_header("Content-Type", e.get_content_type());
						_response.set_body(e.what());
					}
					catch (...)
					{
						response::stock_reply(status_5XX::INTERNAL_SERVER_ERROR, _response);
					}

					if ( _write_timeout > 0 )
					{
						_write_timer.async_wait([this, self](const boost::system::error_code& error) {
							if ( error.value() != boost::system::errc::operation_canceled )
							{
								_connection_manager.stop(shared_from_this());
							}
						});
					}
					do_write();

					try
					{
						_request_handler.on_request_handled(_response, _request);
					}
					catch (...)
					{
					}
				}
				else if ( request_parser_impl::EXPECT_CONTINUE == result )
				{
					// The client is expecting a 100-continue, so we serve it and continue reading.

					response::stock_reply(served::status_1XX::CONTINUE, _response);
					do_write();
				}
				else if ( request_parser_impl::READ_HEADER == result
				       || request_parser_impl::READ_BODY   == result )
				{
					// Not finished reading response, continue.

					do_read();
				}
				else if ( request_parser_impl::REJECTED_REQUEST_SIZE == result )
				{
					// The request is too large and has been rejected

					_status = status_type::DONE;

					response::stock_reply(served::status_4XX::REQ_ENTITY_TOO_LARGE, _response);
					do_write();
				}
				else if ( request_parser_impl::ERROR == result )
				{
					// Error occurred while parsing, respond with BAD_REQUEST

					_status = status_type::DONE;

					response::stock_reply(served::status_4XX::BAD_REQUEST, _response);
					do_write();
				}
			}
			else if (ec != boost::asio::error::operation_aborted)
			{
				_connection_manager.stop(shared_from_this());
			}
		}
	);
}

void
connection::do_write()
{
	auto self(shared_from_this());

	boost::asio::async_write(_socket, boost::asio::buffer(_response.to_buffer()),
		[this, self](boost::system::error_code ec, std::size_t) {
			if ( !ec )
			{
				if ( status_type::READING == _status )
				{
					// If we're still reading from the client then continue
					do_read();
					return;
				}
				else
				{
					// Initiate graceful connection closure.
					boost::system::error_code ignored_ec;
					_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
				}
			}

			if ( ec != boost::asio::error::operation_aborted )
			{
				_connection_manager.stop(shared_from_this());
			}
		}
	);
}
