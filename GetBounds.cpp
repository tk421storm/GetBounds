// GetBounds - Michael Millspaugh
// www.tks-designs.com

#include "Bounds.h"

using namespace std;

static const char* const CLASS = "GetBounds"; // Name of the class (should be the same as the name of the final .dll file)

struct rowValues {
	int first, last;
};

typedef struct rowValues Struct;

void GetBounds::_validate(bool for_real) {
	copy_info(); // Output image will have the same properties as the input

	info_.black_outside(_blackOutside);

	Format format = input0().format();
	Box inputbounds = Box(format.x(), format.y(), format.r(), format.t());
	ImagePlane imagePlane = ImagePlane(inputbounds, true, channels);


	RequestOutput reqData = RequestOutput();
	getRequests(inputbounds, channels, 0, reqData);
	input0().request(channels, 0);

	Interest interest(input0(), inputbounds, imagePlane.channels(), true);
	interest.unlock();

	if (input0().real_valid() && !input0().inErrorState() && !input0().inInvalidState()) {
		if (input0().tryValidate() && interest.valid()) {
			//only do this if the input hash has changed
			running(true);
			getBounds(imagePlane, format, interest);

			Box bounds(p[0], p[1], p[2], p[3]);
			//dont do it if the box is zero or negative (not sure why thats happening)
			if (bounds.area() >= 1) {
				info_.set(bounds);
			};

			_lastHash = input0().hash();
			running(false);

			//Op::warning("Get Bounds set bounding box ");
		}
	}

	//Op::warning("Get Bounds validated ");

}

void GetBounds::getRequests(const Box& box, const ChannelSet& channelsSet, int count, RequestOutput& reqData) {
	return PlanarIop::getRequests(box, channelsSet, count, reqData);
}

void GetBounds::getBounds(ImagePlane& imagePlane, Format& format, Interest& interest) {
	// these useful format variables are used later
	const int fx = format.x();
	const int fy = format.y();

	const int fr = format.r();
	const int ft = format.t();

	const int height = ft - fy;
	const int width = fr - fx;

	bool yMinFound = false;
	bool yMaxFound = false;
	bool xFound = false;
	bool xMaxFound = false;
	//std::cerr << "operating on " + channels << std::endl;

	//for each row
	foreach(z, channels) {
		static int chanNo = imagePlane.chanNo(z);

		static string chanName = getName(z);
		for (int y = imagePlane.bounds().y(); y != imagePlane.bounds().t(); y++) {

			float maxValue = 0;

			//std::cerr << " getBounds on " + chanName + ", row " + to_string(y) << std::endl;

			//walk left to right
			for (int x = imagePlane.bounds().x(); x != imagePlane.bounds().r(); x++) {

				//bounds will never get smaller after a value is found, so we can break once we pass the found point
				//if (xFound && x > p[0]) break;

				float currentPixel = interest.at(x, y, z); //row[z][x]; 
				//make zero if ignoring negatives
				//if (_ignoreNegative && currentPixel < 0) { currentPixel = 0; }
				try {
					if (currentPixel > 0) {
						if (currentPixel > maxValue) maxValue = currentPixel;

						if (!xFound) {
							//std::cerr << " found new max x " + to_string(currentPixel) + " at " + to_string(x) << std::endl;
							p[0] = x - 1;
							xFound = true;
						}
						else {
							if (x < p[0]) { p[0] = x - _extra; }
						}
						break;
					}
				}
				catch (...) {
					//probably attempting to get image before image has actually loaded
					Op::warning("Attempt to GetBounds before source is available");
					abort();
				};
			};

			//walk right to left
			for (int x = imagePlane.bounds().r() + 1; x-- > 0; ) {

				//bounds will never get smaller after a value is found, so we can break once we pass the found point
				//if (xMaxFound && x < p[2]) break;

				float currentPixel = interest.at(x, y, z);
				//make zero if ignoring negatives
				//if (_ignoreNegative && currentPixel < 0) { currentPixel = 0; }

				if (currentPixel > 0) {
					if (currentPixel > maxValue) maxValue = currentPixel;

					if (!xMaxFound) {
						p[2] = x;
						xMaxFound = true;
					}
					else {
						if (x > p[2]) { p[2] = x + _extra; }
					}
					break;


				};
			};

			if (maxValue > 0) {
				if (!yMinFound) {
					p[1] = y - _extra;
					yMinFound = true;
				}
				else {
					p[3] = y + _extra;
				}
			};

			if (maxValue == 0 && yMinFound && yMaxFound) {
				p[3] = y + _extra;
				yMaxFound = true;
			}

			//std::cerr << " current bbox : " + to_string(p[0]) + ", " + to_string(p[1]) + ", " + to_string(p[2]) + ", " + to_string(p[3]) << std::endl;
		};
	};
}

void GetBounds::renderStripe(ImagePlane& imagePlane)
{
	input0().fetchPlane(imagePlane);
	imagePlane.makeUnique();

	//std::cerr << to_string(p[0]) + "," + to_string(p[1]) + "," + to_string(p[2]) + "," + to_string(p[3]) << std::endl;
}
void GetBounds::knobs(Knob_Callback f) {
	ChannelSet_knob(f, &channels, "channels");SetFlags(f, Knob::EARLY_STORE);
	Bool_knob(f, &_blackOutside, "black outside"); SetFlags(f, Knob::STARTLINE); SetFlags(f, Knob::ALWAYS_SAVE); SetFlags(f, Knob::EARLY_STORE);
	Bool_knob(f, &_ignoreNegative, "ignore <0"); SetFlags(f, Knob::EARLY_STORE);
	Int_knob(f, &_extra, "extra");
}

bool GetBounds::renderFullPlanes() {
	//When renderFullPlanes() returns true, the plugin will never be given an image plane to fill that contains less than the full set of channels.
	return true;
}


static Iop* build(Node* node) {
	return new GetBounds(node);
}
const PlanarIop::Description GetBounds::dGet("GetBounds", "GetBounds", build); // Nuke reads this to get the name of your plugin in the UI. Make sure this is the same as the name of the class and the final .dll file!