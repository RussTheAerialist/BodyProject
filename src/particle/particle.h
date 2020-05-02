#pragma once

#include "ofMain.h"
#include "../palette/palette.h"
#include "../data/covidLoader.h"
#include "../clock.h"

const float MAX_CASE_SCALE = 100000.f;

struct ParentReference {
	float *speed;
	PaletteSource *colorPalette;
	glm::vec2 *screenSize;
	LoadedCovidData *data;
};

class Particle {
	public:
		glm::vec2 location;
		glm::vec2 velocity;
		ofColor color;
		glm::vec2 colorLocation;
		const ParentReference& parent;
		float scaledSize = 1.; // TODO: Update Size
		float scaledStep = 0.;
		float size = 0.;
		BucketKey key;

		Particle(const ParentReference &parent, const BucketKey &key)
			: location({0., 0.}), velocity({1., 1.}), colorLocation({1., 1.}), parent(parent), key(key)
		{ updateColor(); }

		void setup() {
			randomize();
		}

		void update(const Clock &clock, bool transitioned) {
			if (transitioned)
				calculateSize(clock);

			float currentSize = scaledSize + scaledStep * clock.transitionPercentage;

			location += velocity * (*parent.speed);

			if (location.x <= scaledSize || location.x >= parent.screenSize->x - currentSize) {
				velocity.x *= -1;
			}

			if (location.y <= scaledSize || location.y >= parent.screenSize->y - currentSize) {
				velocity.y *= -1;
			}
		}

		void calculateSize(const Clock &clock) {
			scaledSize = ofMap(size, 0, MAX_CASE_SCALE, 3, 50);

			int32_t growth = parent.data->getDataFor(clock.currentDate(), key);
			size += growth;
			float nextScaledSize = ofMap(size, 0, MAX_CASE_SCALE, 3, 50);
			scaledStep = nextScaledSize - scaledSize;
		}

		void draw() const {
			ofSetColor(color);
			ofDrawCircle(location.x, location.y, scaledSize);
		}

		void randomize() {
			location.x = ofRandom(parent.screenSize->x);
			location.y = ofRandom(parent.screenSize->y);
			velocity = {ofRandom(10.)-5., ofRandom(10.)-5. };
			glm::vec2 numColors = parent.colorPalette->numColors();
			colorLocation = {ofRandom(numColors.x), ofRandom(numColors.y)};
			updateColor();
		}

	private:
		void updateColor() {
			color = parent.colorPalette->getColorAt(colorLocation);
		}
};