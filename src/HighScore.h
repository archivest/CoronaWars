#pragma once

#include "header.h"

class HighScore
{
public:
    HighScore() :
        mScore(0)
    {
    }

    HighScore(const std::string& initials, unsigned int score) :
        mInitials(initials),
        mScore(score)
    {
    }

    const std::string& getInitials() const
    {
        return mInitials;
    }

    unsigned int getScore() const
    {
        return mScore;
    }

    void setValues(const std::string& initials, unsigned int score)
    {
        mInitials = initials;
        mScore = score;
    }

private:
    std::string mInitials;
    unsigned int mScore;
};
