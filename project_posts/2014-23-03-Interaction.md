## Interaction

To create interaction with visitors of the objects space was planned to use Kinects, placed in the top triangle of icosahedron. After tests found out that 2 Kinects would be enough to grab all needed space. Create standalone app to detect people coming inside object, based on one of examples from ofxCV addon (https://github.com/kylemcdonald/ofxCv) and ofxCv::ContourFinder class with some modifications. App transfers data about people position to main app using OSC, in Icosahedron app data are interpreting to right GL positions. This coordinates creates spheres around visitors.

![contourFinder](../project_images/contourFinderScreen.png "contourFinder")

contourFinder app can be found in project_code folder.

In further development, going to get 4 farest points from the center of detected man's contour to add precice to interaction with virtual space.