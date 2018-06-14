camera {
location  <0, 0, 18>
up        <0,  1,  0>
right     <1.33333, 0,  0>
look_at   <0, 0, 0>
}


light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}

light_source {<-75, 150, 110> color rgb <1.5, 1.0, 1.5>}

// White
sphere { <0, 0, 0>, 2.0
pigment { color rgb <1.0, 1.0, 1.0> }
finish {roughness 0.02 ior 1.6 reflection 1.0}
scale <2, 2, 2>
}

// Green
sphere { <-2.5, -3, 4>, 1.0
pigment { color rgb <1.0, 0.3, 0.3> }
finish {ambient 0.2 diffuse 0.6 specular 0.7 roughness 0.02 ior 1.6 reflection 0.6}
scale <2, 1, 1>
translate <1, 0, 3>
}

// Green
sphere { <-2.5, -3, 4>, 1.0
pigment { color rgb <0.2, 0.5, 0.2> }
finish {ambient 0.2 diffuse 0.6 specular 0.7 roughness 0.02 ior 1.6 reflection 0.6}
scale <1, 2, 1>
rotate <0, 0, -25>
translate <1.2, 8, 0>
}

// Red
sphere { <2.5, -3, 4>, 1.0
pigment { color rgb <0.4, 1.0, 0.4> }
finish {ambient 0.2 diffuse 0.6 specular 0.7 roughness 0.02 ior 1.6 reflection 0.6}
scale <2, 1, 1>
translate <-1, 0, 3>
}


// Red
sphere { <2.5, -3, 4>, 1.0
pigment { color rgb <1.0, 0.3, 0.3> }
finish {ambient 0.2 diffuse 0.6 specular 0.7 roughness 0.02 ior 1.6 reflection 0.6}
scale <1, 2, 1>
rotate <0, 0, 25>
translate <-1.2, 8, 0>
}

// Purple
sphere { <5, -2, 0>, 0.8
pigment { color rgb <0.8, 0.4, 1.0> }
finish {ambient 0.2 diffuse 0.6 specular 0.7 roughness 0.02 ior 1.6 reflection 0.6}
scale <1, 2, 1>
rotate <0, 0, -25>
translate <8.2, 8, 0>
}

// Purple
sphere { <5, -2, 0>, 0.8
pigment { color rgb <0.8, 0.4, 1.0> }
finish {ambient 0.2 diffuse 0.6 specular 0.7 roughness 0.02 ior 1.6 reflection 0.6}
}

// Yellow
sphere { <-5, -2, 0>, 0.8
pigment { color rgb <1.0, 1.0, 0.4> }
finish {ambient 0.2 diffuse 0.6 specular 0.7 roughness 0.02 ior 1.6 reflection 0.6}
scale <1, 2, 1>
rotate <0, 0, 25>
translate <-8.2, 8, 0>
}
// Yellow
sphere { <-5, -2, 0>, 0.8
pigment { color rgb <1.0, 1.0, 0.4> }
finish {ambient 0.2 diffuse 0.6 specular 0.7 roughness 0.02 ior 1.6 reflection 0.6}
}


// Blue
sphere { <-6.5, 0, 4>, 1.0
pigment { color rgb <0.4, 0.6, 1.0> }
finish {ambient 0.2 diffuse 0.6 specular 0.7 roughness 0.02 ior 1.6 reflection 0.6}
scale <1, 2, 1>
}

// Orange
sphere { <6.5, 0, 4>, 1.0
pigment { color rgb <1.0, 0.65, 0.0> }
finish {ambient 0.2 diffuse 0.6 specular 0.7 roughness 0.02 ior 1.6 reflection 0.6}
scale <1, 2, 1>
}


plane {<0, 1, 0>, -4
pigment {color rgb <0.6, 0.7, 0.8>}
finish {ambient 0.4 diffuse 0.8 specular 0.8 roughness 0.001 reflection 0.5}
}

plane {<0, 0, 1>, -6
pigment {color rgb <0.6, 0.7, 0.8>}
finish {ambient 0.4 diffuse 0.8 specular 0.8 roughness 0.001 reflection 0.5}
}