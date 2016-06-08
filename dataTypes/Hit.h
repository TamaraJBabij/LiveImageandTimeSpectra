#ifndef HIT_H
#define HIT_H

/*
	Enum for the different channels in the experiment
*/
enum ChannelID
{
	mcp,
	u1,
	u2,
	v1,
	v2,
	w1,
	w2
};

enum DetectorID
{
    pos,
    neg
};

/*
	1 datum from the experiment
*/

struct Hit
{
    ChannelID channel;
    DetectorID detector;
    int bins;
    double time;
};

#endif
