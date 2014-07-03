// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#ifndef VAQUOLEAPPTHREAD_H
#define VAQUOLEAPPTHREAD_H

#include "VaQuolePublicPCH.h"
#include "VaQuoleWebView.h"

#include <atomic>
#include <mutex>
#include <thread>

#include <QHash>
#include <QList>
#include <QString>
#include <QUuid>

namespace VaQuole
{

class VaQuoleWebPage;

/**
 * Keeps data that we can't store in header because of non-Qt lib use
 */
struct UIDataKeeper
{
	/** Unique object instance id */
	QString ObjectId;

	/** URL managament */
	QString NewURL;

	/** Transparency */
	bool bTransparencyChanged;
	bool bTransparent;

	UIDataKeeper::UIDataKeeper()
		: ObjectId(QUuid::createUuid().toString())
	{}
};

/**
 * Runnable thread manager
 */
class VaThread
{
public:
	VaThread() : m_stop(), m_thread() { }
	virtual ~VaThread() { try { stop(); } catch(...) { /* make something */ } }

	VaThread(VaThread const&) = delete;
	VaThread& operator =(VaThread const&) = delete;

	void stop() { m_stop = true; m_thread.join(); }
	void start() { m_thread = std::thread(&VaThread::run, this); }

protected:
	virtual void run() = 0;
	std::atomic<bool> m_stop;

private:
	std::thread m_thread;
};

/**
 * Class that manages all UI views
 */
class VaQuoleUIManager : public VaThread
{
	// Begin VaThread Interface
protected:
	void run();
	// End VaThread Interface

public:
	void AddPage(VaQuoleWebPage *Page);

private:
	/** Locker to be used with external commands */
	std::mutex mutex;

	/** List of all opened web pages */
	QList<VaQuoleWebPage*> WebPages;

	/** Map of all Qt WebView windows */
	QHash<QString, VaQuoleWebView*> WebViews;

};

} // namespace VaQuole

#endif // VAQUOLEAPPTHREAD_H
