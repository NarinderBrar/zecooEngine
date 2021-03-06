#pragma once
#include "LinearMath/btIDebugDraw.h"

#include "btBulletDynamicsCommon.h"
#include "LinearMath/btIDebugDraw.h"

#include "Line.h"
#include "Material.h"
#include "Camera.h"
#include "SingleLine.h"

using namespace std;

class GLDebugDrawer : public btIDebugDraw
{
    int m_debugMode;
    Line* line;
    Shader* shader;
    Material* material;

    SingleLine* singleLine;

public:
    GLDebugDrawer(Camera* camera);
    virtual ~GLDebugDrawer();
    virtual void drawLine( const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor );
    virtual void drawLine( const btVector3& from, const btVector3& to, const btVector3& color );
    virtual void drawSphere( const btVector3& p, btScalar radius, const btVector3& color );
    virtual void drawBox( const btVector3& bbMin, const btVector3& bbMax, const btVector3& color );
    virtual void drawContactPoint( const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color );
    virtual void reportErrorWarning( const char* warningString );
    virtual void draw3dText( const btVector3& location, const char* textString );
    virtual void setDebugMode( int debugMode );
    virtual int getDebugMode() const;
};