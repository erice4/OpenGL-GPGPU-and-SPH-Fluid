#version 430 core

layout (location=0) in vec4 position;
layout (location=1) in vec4 initialPosition;
layout (location=2) in vec4 velocity;
layout (location=3) in vec2 lifetime;

out vec4 outPosition;
out vec4 outInitialPosition;
out vec4 outVelocity;
out vec2 outLifetime;

vec3 sphereCollision(vec3 ppos, vec3 pvel, vec3 spherePos, float radius)
{
    if (length(ppos-spherePos) < radius)
    {
        vec3 n = normalize(ppos-spherePos);
        vec3 vout = reflect(pvel,n)*0.75f;

        return vout;
    }
    else
    {
        return pvel;
    }
}

void main(void)
{
    outPosition = position;
    outInitialPosition = initialPosition;
    outVelocity = velocity;
    outLifetime = lifetime;

    vec3 spherePos1 = vec3(0,-80,0);
    vec3 spherePos2 = vec3(-30,-140,30);
    vec3 spherePos3 = vec3(20,-170,40);
    vec3 spherePos4 = vec3(0,-200,50);
    vec3 spherePos5 = vec3(-50,-210,52);
    vec3 spherePos6 = vec3(45,-205,55);

    vec3 p = position.xyz;
    vec3 v = velocity.xyz;

    float dt = 0.15f;
    vec3 extForce = vec3(0, -1.8f, 0);
    extForce += -0.1f * v; //Drag

    v += dt * extForce; //Should be extForce / mass

    //Check is hitting a sphere and resolve collision
    v = sphereCollision(p,v,spherePos1, 50);
    v = sphereCollision(p,v,spherePos2, 50);
    v = sphereCollision(p,v,spherePos3, 60);
    v = sphereCollision(p,v,spherePos4, 70);
    v = sphereCollision(p,v,spherePos5, 72);
    v = sphereCollision(p,v,spherePos6, 65);

    p += dt * v;

    outPosition.xyz = p;
    outVelocity.xyz = v;

    outLifetime.x -= dt;

    if (outLifetime.x < 0)
    {
        outLifetime.x = lifetime.y;
        outPosition.xyz = initialPosition.xyz;
        outVelocity = vec4(velocity.w,0,velocity.w,1);
    }
}
