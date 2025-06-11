#include "ServerConfigFallbacks.hpp"
#include "MimeMap.hpp"

// init static map, MimeMap
MimeMap ServerConfigFallbacks::defaultMap("default");

void FallbackInjector::applyProfile(ServerConfigFallbacks& f, const std::string& profile) {
	for (int i = 0; handlers[i].name; ++i) {
		if (profile == handlers[i].name) {
			handlers[i].func(f);
			return;
		}
	}
	std::cerr << "Unknown profile: \"" << profile << "\"."
				 "Falling back to default profile." << std::endl;
	f = ServerConfigFallbacks();
}

ServerConfigFallbacks FallbackInjector::buildWithProfile(const std::string& profile) {
	ServerConfigFallbacks f;
	applyProfile(f, profile);
	return f;
}

const FallbackInjector::ProfileHandler FallbackInjector::handlers[] = {
	{ "default", FallbackInjector::inject_default },
	{ "test",    FallbackInjector::inject_test },
	{ 0,         0 }
};


void	FallbackInjector::inject_default(ServerConfigFallbacks& f) {
}

// dummy
void	FallbackInjector::inject_test(ServerConfigFallbacks& f) {
	f.profile = "test";
	f.keepAlive = false;
	f.client_max_body_size = 999999;
}
