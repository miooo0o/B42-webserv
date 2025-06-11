#include "ServerConfigBase.hpp"

#include "ServerConfigFallbacks.hpp"

ServerConfigBase::ServerConfigBase(const ServerConfigFallbacks& fallbacks)
	: _profile(fallbacks.profile),
	  _root(fallbacks.root),
	  _allowMethods(fallbacks.allowMethods),
	  _indexFiles(fallbacks.indexFiles),
	  _client_max_header_size(fallbacks.client_max_header_size),
	  _client_max_body_size(fallbacks.client_max_body_size),
	  _autoindex(fallbacks.autoindex),
	  _keepAlive(fallbacks.keepAlive),
	  _keepAliveTimeout(fallbacks.keepAliveTimeout),
	  _cgi_support(fallbacks.cgi_support),
	  _chunkedSupport(fallbacks.chunkedSupport),
	  _serverTokens(fallbacks.serverTokens),
	  _cgi_path(fallbacks.cgi_path),
	  _default_type(fallbacks.default_type),
	  _mimeMap(ServerConfigFallbacks::defaultMap),
	  // default constructed, must be set via Parser
	  _serverName(""),
	  _listenAddress(),
	  _redirectInfo()
{
}