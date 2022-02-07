<div id="top"></div>

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a>
    <img src="images/logo.png" alt="Logo" width="200" height="137">
  </a>
  <p align="center">
    An Educational Open Source 3d Graphics Engine
    <br />
    <br />
    <a href="https://github.com/NarinderBrar/zecooEngine/tree/master/zecooEngine/Vendor">Dependencies</a>
    ·
    <a href="https://github.com/NarinderBrar/zecooEngine/issues">Report Bug</a>
    ·
    <a href="https://github.com/NarinderBrar/zecooEngine/issues">Request Feature</a>
  </p>
</div>


<!-- TABLE OF CONTENTS -->

<summary>Table of Contents</summary>
<li><a href="#about-the-project">About The Project</a></li>
<li><a href="#features">Features</a></li>
<li><a href="#screenshots">Screenshots</a></li>
<li><a href="#dependencies">Dependencies</a></li>
<li><a href="#requirements">Requirements</a></li>
<li><a href="#Building">Building Dependencies</a></li>
<li><a href="#contact">Contact</a></li>
<li><a href="#contributing">Contributing</a></li>

## About The Project
![Product Name Screen Shot][product-screenshot]
zecooEngine is an open-source C++ graphics engine mainly developed for educational purposes. It developed while teaching the fundamental of graphics programming. zecooEngine supports OpenGL 3.3 as a graphic API. You can draw 3d models, add lights, apply textures while creating materials. It provides features to add physics on the models using Bullet Physics Engine, can add force, apply constraints, can test collision between bodies.
<p align="right">(<a href="#top">back to top</a>)</p>

## Features
<li>Draw basic primitives like cube, cone, cylinder and sphere etc.</li>
<li>Basic lighting using directional, point and spot dynamic lights.</li>
<li>Draw ray, line for debugging.</li>
<li>Rigid body dynamics: static, dynamic and kinematic bodies.</li>
<li>Collision detection with triggers and callbacks.</li>
<li>Recasting and hit testing.</li>
<li>External 3d model import via Assimp library.</li>
<li>Inputs using mouse, keyboard.</li>
<li>Inputs using mouse, keyboard.</li>
<p align="right">(<a href="#top">back to top</a>)</p>

### Screenshots
Made with zecooEngine
<p align="right">(<a href="#top">back to top</a>)</p>

### Dependencies
zecooEngine use multiple third-party libraries. Here is the full list of dependencies.

<li><b>GLFW</b> - Open Source, multi-platform library for OpenGL, with the API we can create windows, receiving input and events. </li>
<li><b>GLM</b> - is a header only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications. </li>
<li><b>ImGui</b> - is a bloat-free graphical user interface library for C++. </li>
<li><b>AssImp</b> - Open Asset Import Library is a cross-platform 3D model import library which aims to provide a common application programming interface for different 3D asset file formats.</li>
<li><b>FreeImage</b> - is an Open Source library project for developers who would like to support popular graphics image formats like PNG, BMP, JPEG, TIFF and others as needed by today's multimedia applications.</li>
<li><b>Bullet3</b> - real-time collision detection and multi-physics simulation library portable file dialogs - cross-platform file dialog library.</li>
<p align="right">(<a href="#top">back to top</a>)</p>

### Requirements
Software Requirements
<br/>OS : Windows
<br/>IDE : Visual Studio
<br/>CMake

<br/>Hardware Requirements
<br/>Processor : N/A
<br/>Graphics Card : Should supports OpenGL 3.2 and GLSL Shading language 3.3
<br/>Ram : 4 GB or Higher
<p align="right">(<a href="#top">back to top</a>)</p>

### Building
zecooEngine is distributed in source code as a Visual Studio Project 2019. 

Clone this repo to your system using 
git clone https://github.com/NarinderBrar/zecooEngine

<b>Building Dependencies</b>
<b>Build GLFW-3.3.4</b>
<br/>Go to path \zecooEngine\Vendor\glfw-3.3.4
<br/>Make “build” folder
<br/>Use CMake to build Visual Studio Project 2019
<br/>Open GLFW.sln -> Build Solution

<b>Build Bullet Physics 3.17 </b>
<br/>Go to path \zecooEngine\Vendor\bullet3-3.17
<br/>Make “build” dir in Bullet folder
<br/>Use CMake to build Visual Studio Project 2019
<br/>Open ALL_BUILD.vcxproj
<br/>Right click -> App_HelloWorld project -> Set as Startup Project
<br/>Right click -> App_HelloWorld project -> Properties
<br/>C/C++ -> Code Generation -> Runtime Library -> Multi-threaded Debug DLL (/MDd)
<br/>Repeat for BulletCollision, BulletDynamics, LinearMath

<b> Build assimp-3.3.1 </b>
<br/>Go to path \zecooEngine\Vendor\assimp-3.3.1
<br/>Make “build” folder
<br/>Use CMake to build Visual Studio Project 2019
<br/>Open ALL_BUILD.vcxproj
<br/>build gtest
<br/>build assimp
<br/>copy .dll file from \zecooEngine\Vendor\assimp-3.3.1\build\code\Debug to \x64\Debug
<p align="right">(<a href="#top">back to top</a>)</p>

## Contact
If you need any help please feel free to contact me at nsidhubrar@gmail.com
<br/>you can report bugs while creating issue in this repository. 
<p align="right">(<a href="#top">back to top</a>)</p>

## Contributing
As engine developed during the course, So following students contributed and provided assistance in development. 
- <a href="https://github.com/Yuvraj2705">Yuvraj Gupta</a>
- Ritik Bansal
- Vaibhav Sharma
- Ridhampreet Singh Saini
- Uddhav Jindal
- Manav Sharma
- Garvit Nangru
- Achintya Gaur
- Davesh Mehta
- Mandeep Riar
- Sarang Sharma
- Saksham P. Kumar
- Anmol Rattan Kaushal
- Anirudh Banot
- Ankur Goel
- Harneeraj Singh
- Raghav Gupta
- Manas Chakrabarti
- Rajat Sharma
<p align="right">(<a href="#top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
[product-screenshot]: images/productImage.jpg