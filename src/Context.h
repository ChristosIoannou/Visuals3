#pragma once

#include "ofMain.h"
#include <unordered_map>

class Context
{
public:

	Context() : id(ofGetFrameNum()), time(ofGetElapsedTimeMillis()) {};

	template <class T> T getResult(const std::string& name) const
	{
		auto oResult = getResultAny(name);

		if (!oResult.has_value())
			throw std::out_of_range("Result does not exist");

		return std::any_cast<T>(oResult);
	}

	// Helper. For results of type shared_ptr<T>
	//
	// If the result does not exist it returns a null shared ptr.
	// If the result does exist it returns getResults<std::shared_ptr<T>>(name)
	template <class T> std::shared_ptr<T> getResultSharedPtr(const std::string& name) const
	{
		auto oResult = getResultAny(name);
		if (oResult.has_value())
		{
			return std::any_cast<std::shared_ptr<T>>(oResult);
		}

		return nullptr;
	}

	virtual bool hasResult(const std::string& name) const;
	void setResult(const std::string& name, std::any any);
	virtual void clearResult(const std::string& name);
	virtual void clearResults();

private:

	virtual std::any getResultAny(const std::string& name) const;
	std::map<std::string, std::any> results;
	mutable std::mutex resultsMutex;

	const uint64_t id;
	const int64_t time;
};

typedef std::shared_ptr<Context> ContextPtr;

ContextPtr createContext();