#include "CinematicPlayer.h"

CCinematicPlayer::CCinematicPlayer()
    : m_Playing(false)
    , m_CurrentTime(0.0)
    , m_Duration(0.0)
    , m_Cycle(false)
{
}

CCinematicPlayer::~CCinematicPlayer()
{
}

void CCinematicPlayer::Init(float Duration)
{
    m_Duration = Duration;
}

void CCinematicPlayer::Update(float ElapsedTime)
{
    m_CurrentTime += ElapsedTime;
}

void CCinematicPlayer::Stop()
{
    m_Playing = false;
    m_CurrentTime = 0.0;
}

void CCinematicPlayer::Play(bool Cycle)
{
    m_Playing = true;
    m_Cycle = Cycle;
}

void CCinematicPlayer::Pause()
{
    m_Playing = false;
}

void CCinematicPlayer::OnRestartCycle()
{
    m_CurrentTime = 0.0;
}