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
				 ACCEPT_RANGES, // Accept-ranges: bytes
				 AGE, // Age: 12
				 ALLOW, // Allow: GET, HEAD
				 CACHE_CONTROL, // Cache-control: max-age=3600
				 CONNECTION, // Connection: close
				 CONTENT_ENCODING, // Content-encoding: gzip
				 CONTENT_LANGUAGE, // Content-language: en
				 CONTENT_LENGTH, // Content-length: in bytes
				 CONTENT_LOCATION, // Content-location: /index.php|html..
				 CONTENT_MD5, // Content-MD5: hash - base64 sum of the content
				 CONTENT_DISPOSITION, // Content-Disposition: attachment; filename=fname.ext, raise a download box for known MIME TYPES
				 CONTENT_RANGE, // Content-Range: bytes 21010-47021/47022 , something for partial messages
				 CONTENT_TYPE, // Content-Type: text/html; charset=utf-8, mime type of content
				 DATE, // Date: Tue, 15 Nov 1992 09:08:34 GMT, the date and time, when the the document raised
				 ETAG, // ETag: "some hash", indentifier for specific version of resource
				 EXPIRES, // Expires: Tue, 15 Nov 1992 09:08:34 GMT, date and time, when the document is considered stale
				 LAST_MODIFIED, // Last-Modified: Tue, 15 Nov 1992 09:08:34 GMT, date and time, when the document was modified
				 LINK, // Link: </feed>; rel="alternate", express typed relationship with another resource
				 LOCATION, // Location: http://www.example.org/index.php, used for redirection
				 P3P, //P3P: CP="This is not a P3P policy! See http://www.google.com/support/accounts/bin/answer.py?hl=en&answer=151657 for more info." - some bullshit
				 PRAGMA, // Pragma: no-cache, implementation-specific headers that may have various effects anywhere along the request-response chain
				 PROXY_AUTHORIZATION, // Proxy-Authorization: Basic some hash, credential for connecting to a proxy
				 REFRESH, // Refresh: 5; url=http://www.example.org, used in redirection or when a new resources has been created
				 RETRY_AFTER, // Retry-After: 120, if an entity is temporarily unavailable, this instructs the client to try again after specific perid
				 SERVER, // Server: Apache/2.4.1 (UNIX), name for the http server
				 SET_COOKIE, // Set-Cookie: UserID=JohnDoe; Max-Age=3600; Version=1, an http cookie
				 TRAILER, // Trailer: Max-Forwards, the trailer general field value indicates, that the given set of header fields is preesenter in the trailer
				 TRANSFER_ENCODING, // Transfer-Encoding: chunked, the form of encoding used to safely transfer the entity to the user
				 VARY, // Vary: *, Tells downstream proxies how to match future request headers to decide whether the cached response can be used rather than requesting a fresh one from the origin server
				 VIA, // Via: 1.0 fred, 1.1 example.com (Http server), informs the client of proxies through which the response was sent
				 WARNING, // Warning: 199 Miscellaneous warning, general warning about possible problems with the entity body
				 WWW_AUTHENTICATE // WWW-Authenticate: Basic, indicates the authentication scheme that should be used to access the requested entity
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

#define count(x) ( (sizeof(x)) / (sizeof(typeid(x).name())) )

class HttpResponse {

public:
	HttpResponse();

	void addField(uint, string);
	void addField(string, string);

	string headerToString();

	virtual ~HttpResponse();
private:
	map<string, string> fields;

};

#endif	/* HTTPRESPONSE_H */

