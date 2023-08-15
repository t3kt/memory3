uniform uint uMaxEntities;
uniform float uSpawn;
uniform float uDeltaSec;
uniform float uLifespan;


#define sStateIn sTD2DInputs[0]
#define sPositionIn sTD2DInputs[1]
#define sVelocityIn sTD2DInputs[2]
#define sSpawnPosIn sTD2DInputs[3]
#define sForceIn sTD2DInputs[4]

#define STATE_DEAD 0
#define STATE_ALIVE 1

/*layout(binding = 0) */uniform atomic_uint acNextId;
/*layout(binding = 1) */uniform atomic_uint acEntityCount;
uniform atomic_uint acSpawnPulse;

out vec4 stateOut;
layout(location = 1) out vec4 positionOut;
layout(location = 2) out vec4 velocityOut;

float mapRange(float value, float inMin, float inMax, float outMin, float outMax) {
  return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

vec2 mapRange(vec2 value, vec2 inMin, vec2 inMax, vec2 outMin, vec2 outMax) {
  return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

vec3 mapRange(vec3 value, vec3 inMin, vec3 inMax, vec3 outMin, vec3 outMax) {
  return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

vec4 mapRange(vec4 value, vec4 inMin, vec4 inMax, vec4 outMin, vec4 outMax) {
  return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

void main()
{
	vec4 stateVals = texture(sStateIn, vUV.st);
	int state = int(stateVals.x);
	float lifespan = stateVals.y;
	float age = stateVals.z;
	int id = int(stateVals.w);
	vec3 position = texture(sPositionIn, vUV.st).xyz;
	vec3 velocity = texture(sVelocityIn, vUV.st).xyz;
	vec3 force = texture(sForceIn, vUV.st).xyz;

	
	if (state == STATE_ALIVE) {
		age += uDeltaSec;
		if (age >= lifespan) {
			state = STATE_DEAD;
			atomicCounterDecrement(acEntityCount);
		}
	}
	if (state == STATE_DEAD) {
		if (uSpawn > 0. && atomicCounter(acEntityCount) < uMaxEntities && atomicCompSwap(acSpawnPulse, 0, 1) == 0) {
			id = int(atomicCounterIncrement(acNextId));
			atomicCounterIncrement(acEntityCount);
			position = texture(sSpawnPosIn, vUV.st).xyz;
			velocity = vec3(0.);
			force = vec3(0.);
			age = 0.;
			lifespan = uLifespan;
			state = STATE_ALIVE;
		} else {
			id = 0;
			lifespan = 0.;
			age = 0.;
			velocity = vec3(0.);
			force = vec3(0.);
		}
	} else if (state == STATE_ALIVE) {
		//velocity += force * uDeltaSec;
		position += velocity * uDeltaSec;
	}	

	stateOut = TDOutputSwizzle(vec4(float(state), lifespan, age, float(id)));
	positionOut = TDOutputSwizzle(vec4(position, 1.));
	velocityOut = TDOutputSwizzle(vec4(velocity, 1.));
}
