#include "CHYBuilder.h"
#include <cassert>
#include <cstddef>

using namespace std;

std::string BicyclePart::names[NPARTS] = { "Frame", "Wheel", "Seat", "Derailleur", "Handlebar", "Sprocket", "Rack", "Shock" };

void BicycleBuilder::buildFrame() {
	product->addPart(new BicyclePart(BicyclePart::FRAME));
}

void BicycleBuilder::buildWheel() {
	product->addPart(new BicyclePart(BicyclePart::WHEEL));
}

void BicycleBuilder::buildSeat() {
	product->addPart(new BicyclePart(BicyclePart::SEAT));
}

void BicycleBuilder::buildDerailleur() {
	product->addPart(new BicyclePart(BicyclePart::DERAILLEUR));
}

void BicycleBuilder::buildHandlebar() {
	product->addPart(new BicyclePart(BicyclePart::HANDLEBAR));
}

void BicycleBuilder::buildSprocket() {
	product->addPart(new BicyclePart(BicyclePart::SPROCKET));
}

void BicycleBuilder::buildRack() {
}

void BicycleBuilder::buildShock() {
	product->addPart(new BicyclePart(BicyclePart::SHOCK));
}

void TouringBikeBuilder::buildFrame() {
	product->addPart(new BicyclePart(BicyclePart::FRAME));
}

void TouringBikeBuilder::buildWheel() {
	product->addPart(new BicyclePart(BicyclePart::WHEEL));
}

void TouringBikeBuilder::buildSeat() {
	product->addPart(new BicyclePart(BicyclePart::SEAT));
}

void TouringBikeBuilder::buildDerailleur() {
	product->addPart(new BicyclePart(BicyclePart::DERAILLEUR));
}

void TouringBikeBuilder::buildHandlebar() {
	product->addPart(new BicyclePart(BicyclePart::HANDLEBAR));
}

void TouringBikeBuilder::buildSprocket() {
	product->addPart(new BicyclePart(BicyclePart::SPROCKET));
}

void TouringBikeBuilder::buildRack() {
	product->addPart(new BicyclePart(BicyclePart::RACK));
}

void TouringBikeBuilder::buildShock() {
	
}

void RacingBikeBuilder::buildFrame() {
	product->addPart(new BicyclePart(BicyclePart::FRAME));
}

void RacingBikeBuilder::buildWheel() {
	product->addPart(new BicyclePart(BicyclePart::WHEEL));
}

void RacingBikeBuilder::buildSeat() {
	product->addPart(new BicyclePart(BicyclePart::SEAT));
}

void RacingBikeBuilder::buildDerailleur() {
	
}

void RacingBikeBuilder::buildHandlebar() {
	product->addPart(new BicyclePart(BicyclePart::HANDLEBAR));
}

void RacingBikeBuilder::buildSprocket() {
	product->addPart(new BicyclePart(BicyclePart::SPROCKET));
}

void RacingBikeBuilder::buildRack() {
	
}

void RacingBikeBuilder::buildShock() {

}

void BicycleTechnician::construct() {
	assert(builder);
	builder->createProduct();
	builder->buildFrame();
	builder->buildWheel();
	builder->buildSeat();
	builder->buildDerailleur();
	builder->buildHandlebar();
	builder->buildSprocket();
	builder->buildRack();
	builder->buildShock();
}
