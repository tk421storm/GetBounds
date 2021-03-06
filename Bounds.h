#pragma once

#include "DDImage/PlanarIop.h"
#include "DDImage/Row.h"
#include "DDImage/Knobs.h"
#include "DDImage/NukeWrapper.h"
#include "DDImage/Interest.h"
#include "DDImage/Thread.h"
#include "DDImage/ArrayKnobI.h"
#include "DDImage/MetaData.h"

using namespace DD::Image;

static const char* const HELP = "auto bbox"; // The tooltip you see when hovering above the Help-button in Nuke
static const char* const InputLabel = "1"; 

class GetBounds : public PlanarIop {
	//int value[3]; // Create user variable for per-channel translation
	ChannelSet channels = ChannelSet();

	Box bbox = Box(0, 0, 1, 1);

	int p[4];

	bool _blackOutside;

	bool _ignoreNegative;

	bool _firstTime;
	Lock _lock;
	Hash _lastHash;

	//MetaData::Bundle meta;

	int _extra;

	//GetBounds(Node* node);
	//GetBounds(NodeI* node);

	//struct walkRow();

public:

	GetBounds(Node* node) : PlanarIop(node) { // Set all default values here
		//value[0] = value[1] = value[2] = 0; // For instance, all items in the int value[3] should be 0 by default

		Box inputFormat = input_format();
		p[0] = inputFormat.x();
		p[1] = inputFormat.y();
		p[2] = inputFormat.r();
		p[3] = inputFormat.t();

		_blackOutside = true;
		_ignoreNegative = false;
		_lastHash = Hash();
		_extra = 0;
	}
	virtual ~GetBounds() {};

	void _validate(bool); // This will define the output image, like the size and channels it will have
	void getRequests(const Box& box, const ChannelSet& channels, int count, RequestOutput& reqData); // This requests information from the input
	void renderStripe(ImagePlane& imagePlane); // Where the calculations take place
	virtual bool renderFullPlanes();

	void getBounds(ImagePlane& imagePlane, Format& format, Interest& interest);

	const char* Class() const { return dGet.name; }
	const char* node_help() const { return HELP; }
    const char* input_label(int n, char*) const { return InputLabel; }

	virtual void knobs(Knob_Callback); // This is where knobs can be assigned
	static const PlanarIop::Description dGet; // Make Nuke understand this node (at the bottom of the script more information on this)
};