# Cinder-Voronoi

This is mostly a quick Cinder port of [ofxVoronoi](https://github.com/madc/ofxVoronoi).

The block allows the generation of simple two-dimensional voronoi diagrams. It uses a modified version of [Stephan Fortune's sweep line algorithm](http://ect.bell-labs.com/who/sjf/) written by Chris H. Rycroft for the [Voro++ project](http://math.lbl.gov/voro++/).

Now with added support for [Lloyd's algorithm](https://en.wikipedia.org/wiki/Lloyd%27s_algorithm) useful for relaxing the diagram for use with stippling effects.

![sample](https://github.com/redpaperheart/Cinder-Voronoi/raw/master/screenshot.png)
