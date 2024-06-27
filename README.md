## Waz-API
Waz aims to use a monocular camera to do path planning in the image space (like our friend Mike Wazowkski… a stretch, I know). We do this by taking advantage of semantic segmentation and relative depth models. We can use segmentation to inform what is traversable and use relative depth to capture objects that the robot could hit, like potholes and ditches. These models inform a smarter cost map in the image space. The cost map is then used to plan a path in the image space which is then converted into a velocity commands on the robot.

