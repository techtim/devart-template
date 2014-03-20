# Icosahedron

## Authors
- Tim Tavlintsev, https://github.com/techtim
- Anton Podkopaev

## Description
Research of possibility of creating physical space, where virtual 3D scene will cross reality, not in usual 2D projection, but in three dimensional physical space. Virtual space creates in custom made software using openFrameworks. Space consists Icosahedron object itself with simple geometric shapes (pyramids, spheres, cubes, etc.) that moves around and deforms, depending on real word factors, for example sounds or human position inside space. On the real object light appears in places where shapes cross wireframe of virtual Icosahedron. Audience dive into the space that never exists before, becouse light forms our perceive of it. Interesting to explore how this 3d space can interact and deliver emotional feedback to audience inside it.

## Link to Prototype
[Project Page](http://tvl.io/project/projecticosahedron "Project Page")

## Example Code
Capturing of sides states uses OpenGl Stencil buffer to cut scene with Icosahedron volume
```
void testApp::drawInIco() {
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_NEVER, 1, 0xFF);
    glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)
    
    // draw stencil pattern
    glStencilMask(0xFF);
    glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF
    icoMesh.draw();
    
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilMask(0x00);
    // draw where stencil's value is 0
    glStencilFunc(GL_EQUAL, 0, 0xFF);
    /* (nothing to draw) */
    // draw only where stencil's value is 1
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    
    drawScene(false);
    
    glDisable(GL_STENCIL_TEST);
    
}
```


[openFrameworks](https://github.com/openframeworks/openFrameworks "openFrameworks")
[ofxOsc](https://github.com/roxlu/ofxOsc "ofxOsc")
[ofxKinect](https://github.com/ofTheo/ofxKinect‎ "ofxKinect‎")
[ofxCv](https://github.com/kylemcdonald/ofxCv "ofxCv")
[OpenCV](https://github.com/Itseez/opencv "OpenCV")

## Images & Videos

![Icosahedron](project_images/cover.jpg?raw=true "Icosahedron")
![People inside](project_images/ico-hand.jpg?raw=true "People dancing inside")
![People siting](project_images/ico-people.jpg?raw=true "People sitting inside")
![Icosahedron on dancefloor](http://tvl.io/i/project/projecticosahedron/70180_icoauxwide.jpeg?raw=true "Icosahedron on dancefloor")
![project Icosahedron](http://www.youtube.com/embed/3AoSZ6N3uQk?list=UUUMPgeNiY0WnSZCiyxHOquQ "project:Icosahedron video")

