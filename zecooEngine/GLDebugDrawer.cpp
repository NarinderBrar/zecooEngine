#include "GLDebugDrawer.h"

GLDebugDrawer::GLDebugDrawer( Camera* camera ) : m_debugMode( 0 )
{
    shader = new Shader( "resources\\shader\\unlit.vs", "resources\\shader\\unlit.fs" );

    glm::vec4 color = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );

    material = new Material( shader, color );
    material->linkCamera( camera );

    singleLine = new SingleLine( glm::vec3( 0, 0, 0 ), glm::vec3( 0, 0, 0 ) );

}

GLDebugDrawer::~GLDebugDrawer()
{
}

void GLDebugDrawer::drawLine( const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor )
{
    glm::mat4 p = glm::mat4( 1.0 );
    material->shader->use();
    material->shader->setMat4( "model", p);
    material->update();
    singleLine->Update( glm::vec3( from.getX(), from.getY(), from.getZ() ), glm::vec3( to.getX(), to.getY(), to.getZ() ) );
    singleLine->draw();
}

void GLDebugDrawer::drawLine( const btVector3& from, const btVector3& to, const btVector3& color )
{
    drawLine( from, to, color, color );
}

void GLDebugDrawer::drawSphere( const btVector3& p, btScalar radius, const btVector3& color )
{
   // gl::color( Color( color.getX(), color.getY(), color.getZ() ) );
   // gl::drawSphere( Vec3f( p.getX(), p.getY(), p.getZ() ), radius );
}

void GLDebugDrawer::drawBox( const btVector3& bbMin, const btVector3& bbMax, const btVector3& color )
{
   // gl::color( Color( color.getX(), color.getY(), color.getZ() ) );
    //gl::drawStrokedCube( AxisAlignedBox3f(Vec3f( bbMin.getX(), bbMin.getY(), bbMin.getZ() ),Vec3f( bbMax.getX(), bbMax.getY(), bbMax.getZ() ) ) );
}

void GLDebugDrawer::drawContactPoint( const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color )
{
    /*Vec3f from( PointOnB.getX(), PointOnB.getY(), PointOnB.getZ() );
    Vec3f to( normalOnB.getX(), normalOnB.getY(), normalOnB.getZ() );
    to = from + to * 1;

    gl::color( Color( color.getX(), color.getY(), color.getZ() ) );
    gl::begin( GL_LINES );
    gl::vertex( from );
    gl::vertex( to );
    gl::end();*/
}

void GLDebugDrawer::reportErrorWarning( const char* warningString )
{
    
}

void GLDebugDrawer::draw3dText( const btVector3& location, const char* textString )
{

}

void GLDebugDrawer::setDebugMode( int debugMode )
{
    m_debugMode = debugMode;
}

int GLDebugDrawer::getDebugMode() const
{
    return m_debugMode;
}