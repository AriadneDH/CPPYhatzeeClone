# CPPYhatzeeClone
 Clone of the game Yhatzee written primarily in C++ with an JS GUI
Project is completed in two steps. The first is to get a working logic + abstraction framework created in c++ that stands up to testing
copies the functions of yhatzee, and Is resilient to be able to handel improper interactions without any undefined behavior.

Additionally while this could be done with functional style coding, it is done in an object oriented fasion for implementation in 
phase two, and to be able to seemlessly make multiple player instances.
And the reason for using c++ for the oop implementation as oposed to java is with reason as well.

Step two is taking the classes (all of which are in a single file in part one) and re-implementing them for
WASM(WebAssembly) conversion. The code will be cleaned up and have incressed efficiency, the random implementation
for dice rolling will be changed as you can't grab the time value in order to generate psyudo-rand numbers in wasm alone.
And the c++ class system of gameState < dice + score will be implemented as a logic system that is called with java script
This will allow a web implementation of yhatzee with multiple players and a refined front end interface using a framework/library such
as vue, react, or electron to make a pretty interface. All of which can be accessed without the need to compile c++ code
