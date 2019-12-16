#ifndef GAMETIMER_H
#define GAMETIMER_H

class GameTimer
{
public:
	GameTimer();
	// 总游戏时间
	float TotalTime() const;
	// 帧间隔时间
	float DeltaTime() const;
	// 在消息循环之前调用
	void Reset();
	// 在取消暂停的时候调用
	void Start();
	// 在暂停的时候调用
	void Stop();
	// 在每一帧的时候调用
	void Tick();

private:
	double m_SecondsPerCount;
	double m_DeltaTime;

	__int64 m_BaseTime;
	__int64 m_PausedTime;
	__int64 m_StopTime;
	__int64 m_PrevTime;
	__int64 m_CurrTime;

	bool m_Stopped;
};

#endif // !GAMETIMER_H
