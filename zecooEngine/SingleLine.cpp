#include "SingleLine.h"
SingleLine::SingleLine( glm::vec3 start, glm::vec3 end )
{
    startPoint = start;
    endPoint = end;

    vertices = {
         start.x, start.y, start.z,
         end.x, end.y, end.z,
    };

    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glBindVertexArray( VAO );

    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices.data(), GL_DYNAMIC_DRAW );

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), (void*)0 );
    glEnableVertexAttribArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}
void SingleLine::Update( glm::vec3 start, glm::vec3 end )
{
    vertices = {
     start.x, start.y, start.z,
     end.x, end.y, end.z,
    };
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices.data(), GL_DYNAMIC_DRAW );
}

int SingleLine::draw()
{
    glBindVertexArray( VAO );
    glDrawArrays( GL_LINES, 0, 2 );
    return 0;
}

SingleLine::~SingleLine()
{
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    glDeleteProgram( shaderProgram );
}