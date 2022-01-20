// SetBounds - Max van Leeuwen - Copyright (C) 2017.
// Example project for C++ compiling.
// See www.maxvanleeuwen.com/nuke-ndk for more information.

#include "Bounds.h"

using namespace DD::Image;
using namespace std;

static const char* const CLASS = "SetBounds"; // Name of the class (should be the same as the name of the final .dll file)



void SetBounds::_validate(bool for_real) {
	copy_info(); // Output image will have the same properties as the input

	if (!getbounds) {
		//getbounds = GetBounds::create("GetBounds");
		//getbounds = &GetBounds(thisNode);
		//GetBounds* getbounds = dynamic_cast<GetBounds*>(getbounds);
		PlanarIop* getbounds = dynamic_cast<PlanarIop*>(PlanarIop::create("Blur"));
	}
	if (!getbounds) Op::error("failed to create internal op GetBounds");

	getbounds->set_input(0, input0());

	//getbounds->invalidate();
	getbounds->validate(for_real);

	//getbounds->request(info_.x(), info_.y(), info_.r(), info_.t(), info_.channels(), 1);
	Box bounds = Box(info_.x(), info_.y(), info_.r(), info_.t());
	RequestOutput reqData = RequestOutput();
	getbounds->getRequests(bounds, channels, 1, reqData);

	ImagePlane image = ImagePlane(bounds, true, channels);

	getbounds->renderStripe(image);

	//getbounds->getResult(p);
	//Box bounds(50, 50, 100, 100);
	//info_.set(bbox);
	channels = input0().channels();
	Op::warning("Set Bounds validating ");
	Op::warning(to_string(p[0]).c_str());
	Op::warning(to_string(p[1]).c_str());
	Op::warning(to_string(p[2]).c_str());
	Op::warning(to_string(p[3]).c_str());

}
void SetBounds::getRequests(const Box& box, const ChannelSet& channels, int count, RequestOutput& reqData) {
	return PlanarIop::getRequests(box, channels, count, reqData);
}

void SetBounds::renderStripe(ImagePlane& imagePlane) {
	input0().fetchPlane(imagePlane);
	imagePlane.makeUnique();
	/*
	imagePlane.makeWritable();
	Box box = imagePlane.bounds();

	foreach(z, imagePlane.channels()) {
		for (Box::iterator it = box.begin(); it != box.end(); it++) {
			imagePlane.writableAt(it.x, it.y, imagePlane.chanNo(z)) *= 1;
		}
	}
	*/
	//return PlanarIop::renderStripe(imagePlane);
}
void SetBounds::knobs(Knob_Callback f) {
	Knob* bounds = MultiInt_knob(f, p, 4, "bounds", "AutoCrop"); // Make the user knob
	SetFlags(f, Knob::OUTPUT_ONLY);
	if (f.makeKnobs()) {
		SetValueProvider(f, this);
	}
	Tooltip(f, "Data from the calculation"); // Set the tooltip for this knob
}
/*
int SetBounds::knob_changed(Knob* k)
{
	//knob("bounds")->set_values(p, 4);
	return PlanarIop::knob_changed(k);
}
bool SetBounds::updateUI(OutputContext out)
{
	//knob("bounds")->set_values(p, 4);
	return PlanarIop::updateUI(out);
}
*/
bool SetBounds::renderFullPlanes() {
	//When renderFullPlanes() returns true, the plugin will never be given an image plane to fill that contains less than the full set of channels.
	return true;
}

bool SetBounds::provideValuesEnabled(const ArrayKnobI* arrayKnob, const OutputContext& oc) const {
	return true;
}

vector<double> SetBounds::provideValues(const ArrayKnobI* arrayKnob, const OutputContext& oc) const {
	std::vector<double> values;
	values.push_back(p[0]);
	values.push_back(p[1]);
	values.push_back(p[2]);
	values.push_back(p[3]);
	return values;
}

static Iop* build(Node* node) {
	return (new NukeWrapper(new SetBounds(node)))->noMix()->noMask();
}
const PlanarIop::Description SetBounds::dSet("SetBounds", "SetBounds", build); // Nuke reads this to get the name of your plugin in the UI. Make sure this is the same as the name of the class and the final .dll file!