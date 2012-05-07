/*
 * File:   HttpResponse.h
 * Author: solution
 *
 * Created on 7. květen 2012, 9:31
 */

#ifndef HTTPRESPONSE_H
#define	HTTPRESPONSE_H

#include <iostream>
#include <map>
#include <typeinfo>

using namespace std;

typedef unsigned int uint;

enum FieldTypes {
				 ACCEPT_RANGES, /* Accept-ranges: bytes */
				 AGE, /* Age: 12 */
				 ALLOW, /* Allow: GET, HEAD */
				 CACHE_CONTROL, /* Cache-control: max-age=3600 */
				 CONNECTION, /* Connection: close */
				 CONTENT_ENCODING, /* Content-encoding: gzip */
				 CONTENT_LANGUAGE, /* Content-language: en */
				 CONTENT_LENGTH, /* Content-length: in bytes */
				 CONTENT_LOCATION, /* Content-location: /index.php|html.. */
				 CONTENT_MD5, /* Content-MD5: hash - base64 sum of the content */
				 CONTENT_DISPOSITION, /* Content-Disposition: attachment; filename=fname.ext, raise a download box for known MIME TYPES */
				 CONTENT_RANGE, /* Content-Range: bytes 21010-47021/47022 , something for partial messages */
				 CONTENT_TYPE, /* Content-Type: text/html; charset=utf-8, mime type of content */
				 DATE, /* Date: Tue, 15 Nov 1992 09:08:34 GMT, the date and time, when the the document raised */
				 ETAG, /* ETag: "some hash", indentifier for specific version of resource */
				 EXPIRES, /* Expires: Tue, 15 Nov 1992 09:08:34 GMT, date and time, when the document is considered stale */
				 LAST_MODIFIED, /* Last-Modified: Tue, 15 Nov 1992 09:08:34 GMT, date and time, when the document was modified */
				 LINK, /* Link: </feed>; rel="alternate", express typed relationship with another resource */
				 LOCATION, /* Location: http://www.example.org/index.php, used for redirection */
				 P3P, /*P3P: CP="This is not a P3P policy! See http://www.google.com/support/accounts/bin/answer.py?hl=en&answer=151657 for more info." - some bullshit */
				 PRAGMA, /* Pragma: no-cache, implementation-specific headers that may have various effects anywhere along the request-response chain */
				 PROXY_AUTHORIZATION, /* Proxy-Authorization: Basic some hash, credential for connecting to a proxy */
				 REFRESH, /* Refresh: 5; url=http://www.example.org, used in redirection or when a new resources has been created */
				 RETRY_AFTER, /* Retry-After: 120, if an entity is temporarily unavailable, this instructs the client to try again after specific perid */
				 SERVER, /* Server: Apache/2.4.1 (UNIX), name for the http server */
				 SET_COOKIE, /* Set-Cookie: UserID=JohnDoe; Max-Age=3600; Version=1, an http cookie */
				 TRAILER, /* Trailer: Max-Forwards, the trailer general field value indicates, that the given set of header fields is preesenter in the trailer */
				 TRANSFER_ENCODING, /* Transfer-Encoding: chunked, the form of encoding used to safely transfer the entity to the user */
				 VARY, /* Vary: *, Tells downstream proxies how to match future request headers to decide whether the cached response can be used rather than requesting a fresh one from the origin server */
				 VIA, /* Via: 1.0 fred, 1.1 example.com (Http server), informs the client of proxies through which the response was sent */
				 WARNING, /* Warning: 199 Miscellaneous warning, general warning about possible problems with the entity body */
				 WWW_AUTHENTICATE /* WWW-Authenticate: Basic, indicates the authentication scheme that should be used to access the requested entity */
		   };

static const char *fieldStringTypes[] = {
	"Accept-Ranges:",
	"Age:",
	"Allow:",
	"Cache-Control:",
	"Connection:",
	"Content-Encoding:",
	"Content-Language:",
	"Content-Location:",
	"Content-Length:",
	"Content-MD5:",
	"Content-Disposition:",
	"Content-Range:",
	"Content-Type:",
	"Date:",
	"ETag:",
	"Expires:",
	"Last-Modified:",
	"Link:",
	"Location:",
	"P3P:",
	"Pragma:",
	"Proxy-Authenticate:",
	"Refresh:",
	"Retry-After:",
	"Server:",
	"Transfer-Encoding:",
	"Vary:",
	"Warning:",
	"WWW-Authenticate:"
};


enum HttpStates {
				  /* 2xx success codes */
				  OK = 200, /* Standart succesful http request */

				  CREATED = 201, /* The request has been fulfilled and resulted <br />in a new resource being created */

				  ACCEPTED = 202, /* The request has been accepted for processing, but the <br />
								   * processing has not been completed
								   */

				  NON_AUTHORITATIVE_INFORMATION = 203, /* The server succesfully processed the <br />
													   * request, but is returning info that may be from another source
													   */

				  NO_CONTENT = 204, /* The server successfully processed the request, but is not <br />
								   * returning any content
								   */

				  RESET_CONTENT = 205, /* The server successfully processed the request, but is not <br />
									   * returning any content
									   */

				  PARTIAL_CONTENT = 206, /* The server is delivering only part of the resource due <br />
									   * to a range header sent by the client.
									   */

				  MULTI_STATUS = 207, /* The message body that follows is an XML message and can contain a number <br />
									   * of separate response codes, depending on how many sub-requests were made
									   */

				  ALREADY_REPORTED = 208, /* The members of a DAV binding have already been enumerated <br />
										   * in a previous reply to this request, <br />and are not being included again.
										   */

				  IM_USED = 226, /* The server has fulfilled a GET request for the resource,
							   * <br /> and the response is a representation of the result of one or
							   * <br /> more instance-manipulations applied to the current instance.
							   */

				  /* 3xx redirection codes */

				  MULTPLE_CHOICES = 300, /* Indicates multiple options for the resource that the<br />
									   * client may follow. It, for instance, could be used to present<br />
									   * different format options for video, <br />list files with different
									   * extensions, or word sense disambiguation.
									   */

				  MOVED_PERMANENTLY = 301, /* This and all future requests should be directed to the given URI. */

				  FOUND = 302, /* This is an example of industry practice contradicting the standard.<br />
							   * The HTTP/1.0 specification (RFC 1945) required the client to<br />
							   * perform a temporary redirect
							   */

				  SEE_OTHER = 303, /* The response to the request can be found under another URI <br />
								   * using a GET method. When received in response to a POST (or PUT/DELETE),<br />
								   * it should be assumed that the server has received the data and the <br />
								   * redirect should be issued with a separate GET message.
								   */

				  NOT_MODIFIED = 304, /* Indicates the resource has not been modified since last requested. <br />
									   * Typically, the HTTP client provides a header like the If-Modified-Since<br />
									   * header to provide a time against which to compare. Using this saves <br />
									   * bandwidth and reprocessing on both the server and client, as only the <br />
									   * header data must be sent and received in comparison to the entirety of <br />
									   * the page being re-processed by the server, then sent again using more <br />
									   * bandwidth of the server and client.
									   */

				  USE_PROXY = 305, /* Many HTTP clients (such as Mozilla and Internet Explorer) do not correctly <br />
								   * handle responses with this status code, primarily for security reasons.
								   */

				  SWITCH_PROXY = 306, /* No longer used. Originally meant "Subsequent requests should use the specified proxy."*/

				  TEMPORARY_REDIRECT = 307, /* In this case, the request should be repeated with another URI; <br />
										   * however, future requests can still use the original URI. In contrast<br />
										   * to 302, the request method should not be changed when reissuing the<br />
										   * original request. For instance, a POST request must be repeated using another POST request.
										   */

				  PERMANENT_REDIRECT = 308, /* The request, and all future requests should be repeated using another URI.<br />
										   * 307 and 308 (as proposed) parallel the behaviours of 302 and 301, but do not <br />
										   * require the HTTP method to change. So, for example, submitting a form to a <br />
										   * permanently redirected resource may continue smoothly.
										   */

				  /* 4xx client error */

				  NOT_FOUND = 400,
				  }

#define count(x) ( (sizeof(x)) / (sizeof(typeid(x).name())) )

class HttpResponse {

public:
	HttpResponse();

	void addField(uint, string);
	void addField(string, string);
	void prepend(string);
	void prepend(string);

	string headerToString();

	virtual ~HttpResponse();
private:
	map<string, string> fields;

};

#endif	/* HTTPRESPONSE_H */

