#ifndef GAMETIMER_H
#define GAMETIMER_H

class GameTimer
{
public:
	GameTimer();
	// ����Ϸʱ��
	float TotalTime() const;
	// ֡���ʱ��
	float DeltaTime() const;
	// ����Ϣѭ��֮ǰ����
	void Reset();
	// ��ȡ����ͣ��ʱ�����
	void Start();
	// ����ͣ��ʱ�����
	void Stop();
	// ��ÿһ֡��ʱ�����
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
