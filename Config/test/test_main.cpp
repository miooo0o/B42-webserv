int main () {

	ServerConfigFallbacks fallback = FallbackInjector::buildWithProfile("default");
	ServerConfigBase config(fallback);

	ConfigParser parser;
	parser.overrideWithParsedValues(config, tokens);
	ServerConfigBase config.override(parser.parseAndBuild(configFallbacks, tokens));

}