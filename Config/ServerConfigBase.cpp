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

ServerConfigBase::ServerConfigBase(const ParseResult& result)
	: _listenAddress(), _redirectInfo()
{
	const ServerConfigFallbacks fallbacks;
	_profile = fallbacks.profile;
	_root = fallbacks.root;
	_allowMethods = fallbacks.allowMethods;
	_indexFiles = fallbacks.indexFiles;
	_client_max_header_size = fallbacks.client_max_header_size;
	_client_max_body_size = fallbacks.client_max_body_size;
	_autoindex = fallbacks.autoindex;
	_keepAlive = fallbacks.keepAlive;
	_keepAliveTimeout = fallbacks.keepAliveTimeout;
	_cgi_support = fallbacks.cgi_support;
	_chunkedSupport = fallbacks.chunkedSupport;
	_serverTokens = fallbacks.serverTokens;
	_cgi_path = fallbacks.cgi_path;
	_default_type = fallbacks.default_type;
	// _mimeMap = ServerConfigFallbacks::defaultMap;
	// default constructed, must be set via Parser
	_serverName = "";
	overrideWithOrThrow(result);
}

// ============================================================================================================
// private
// ============================================================================================================

void	ServerConfigBase::overrideWith(const ParseResult& result) {
	if (result.type != "server")
		throw std::logic_error();
	if (result.directives.empty())
		throw std::logic_error();
	for (int i = 0; i < result.directives.size(); i++) {}

	for (int i = 0; i < result.children.size(); i++) {
		overrideWith(result.children[i], *this);
	}

}

void	ServerConfigBase::overrideWith(const ParseResult& result, ServerConfigBase& config) {
	if (result.type != "location")
		throw std::logic_error();
	if (result.directives.empty())
		throw std::logic_error();
	if (result.directives[0].args.size() != 1)
		throw std::logic_error("Location directive must have exactly one path argument");
	std::string	location_path = result.directives[0].args[0];
	Location location;
	for (int i = 1; i < result.directives.size(); i++) {

	}
}


// ============================================================================================================
// [Getter]
// ============================================================================================================


// ============================================================================================================
// [Setter]
// ============================================================================================================
