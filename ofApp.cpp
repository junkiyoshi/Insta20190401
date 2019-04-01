#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(39);
	ofSetLineWidth(1.5);
	ofNoFill();

	this->frame_span = 30;
	this->size = 15;
	while (this->locations.size() < 150) {

		auto location = glm::vec2(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
		bool flag = true;
		for (int i = 0; i < this->locations.size(); i++) {

			if (glm::distance(location, this->locations[i]) < this->size * 3) {

				flag = false;
				break;
			}
		}

		if (flag) {

			this->locations.push_back(location);
		}
	}

	for (int i = 0; i < 30; i++) {
	
		this->current_indexes.push_back(0);
		this->next_indexes.push_back(ofRandom(this->locations.size()));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < this->current_indexes.size(); i++) {

		if (ofGetFrameNum() % (this->frame_span * 2) == 0) {

			int next_index = this->next_indexes[i];
			int current_index = next_index;

			vector<int> near_indexes;
			for (int location_index = 0; location_index < this->locations.size(); location_index++) {

				if (current_index == location_index) { continue; }
				if (glm::distance(this->locations[current_index], this->locations[location_index]) < this->size * 8) {

					near_indexes.push_back(location_index);
				}
			}

			if (near_indexes.size() > 0) {

				next_index = near_indexes[ofRandom(near_indexes.size())];
			}

			this->current_indexes[i] = current_index;
			this->next_indexes[i] = next_index;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	int frame_param = ofGetFrameNum() % this->frame_span;

	for (int i = 0; i < this->next_indexes.size(); i++) {

		int current_index = this->current_indexes[i];
		int next_index = this->next_indexes[i];

		auto angle_current = std::atan2(this->locations[next_index].y - this->locations[current_index].y, this->locations[next_index].x - this->locations[current_index].x);
		auto satellite_point_current = this->locations[current_index] + glm::vec2(this->size * cos(angle_current), this->size * sin(angle_current));
		auto angle_next = std::atan2(this->locations[current_index].y - this->locations[next_index].y, this->locations[current_index].x - this->locations[next_index].x);
		auto satellite_point_next = this->locations[next_index] + glm::vec2(this->size * cos(angle_next), this->size * sin(angle_next));

		if (ofGetFrameNum() % (this->frame_span * 2) < this->frame_span) {

			ofDrawLine(satellite_point_current, satellite_point_next);
			for (int location_index = 0; location_index < this->locations.size(); location_index++) {

				ofDrawCircle(this->locations[location_index], this->size);

				if (current_index == location_index) {

					int deg_param = ofMap(frame_param, 0, this->frame_span, 180, 0);
					float start_deg = angle_current * RAD_TO_DEG;

					ofFill();
					ofBeginShape();
					for (int deg = start_deg; deg <= start_deg + deg_param; deg++) {

						ofVertex(this->locations[location_index] + glm::vec2(this->size * cos(deg * DEG_TO_RAD), this->size * sin(deg * DEG_TO_RAD)));
					}
					for (int deg = start_deg - deg_param; deg <= start_deg; deg++) {

						ofVertex(this->locations[location_index] + glm::vec2(this->size * cos(deg * DEG_TO_RAD), this->size * sin(deg * DEG_TO_RAD)));
					}

					ofEndShape();
					ofNoFill();
				}

				if (next_index == location_index) {

					int deg_param = ofMap(frame_param, 0, this->frame_span, 0, 180);
					float start_deg = angle_next * RAD_TO_DEG;

					ofFill();
					ofBeginShape();
					for (int deg = start_deg; deg <= start_deg + deg_param; deg++) {

						ofVertex(this->locations[location_index] + glm::vec2(this->size * cos(deg * DEG_TO_RAD), this->size * sin(deg * DEG_TO_RAD)));
					}
					for (int deg = start_deg - deg_param; deg <= start_deg; deg++) {

						ofVertex(this->locations[location_index] + glm::vec2(this->size * cos(deg * DEG_TO_RAD), this->size * sin(deg * DEG_TO_RAD)));
					}

					ofEndShape();
					ofNoFill();
				}
			}
		}
		else {

			auto distance = glm::distance(satellite_point_next, satellite_point_current);
			distance = ofMap(frame_param, this->frame_span, this->frame_span * 2, 0, distance);
			auto direction = satellite_point_next - satellite_point_current;
			ofDrawLine(satellite_point_next, satellite_point_next + glm::normalize(direction) * distance);

			for (int location_index = 0; location_index < this->locations.size(); location_index++) {

				ofDrawCircle(this->locations[location_index], this->size);
			}

			ofFill();
			ofDrawCircle(this->locations[next_index], this->size);
			ofNoFill();
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}