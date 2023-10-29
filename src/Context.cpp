#include "Context.h"

bool Context::hasResult(const std::string& name) const 
{
	std::lock_guard<std::mutex> lk(resultsMutex);
	return results.find(name) != results.end();
}

std::any Context::getResultAny(const std::string& name) const
{
	std::lock_guard<std::mutex> lk(resultsMutex);
	auto itr = results.find(name);

	if (itr != results.end())
		return itr->second;

	return std::any();
}

void Context::setResult(const std::string& name, std::any any) 
{
	std::lock_guard<std::mutex> lk(resultsMutex);
	results[name] = std::move(any);
}

void Context::clearResult(const std::string& name) 
{
	std::lock_guard<std::mutex> lk(resultsMutex);
	auto it = results.find(name);
	if (results.end() == it)
		throw std::runtime_error(__FUNCTION__ " result does not exist!");
	results.erase(it);
}

void Context::clearResults() {
	std::lock_guard<std::mutex> lk(resultsMutex);

	// Remove all results which are not flagged persistAfterClear
	for (auto itr = results.begin(); itr != results.end();)
	{
		itr = results.erase(itr);
	}
}

std::shared_ptr<Context> createContext() {
	return std::make_shared<Context>();
}