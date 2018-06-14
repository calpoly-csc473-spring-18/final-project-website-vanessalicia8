// p3: custom file

camera {
location  <0, 0, 12>
up        <0,  1,  0>
right     <1.33333, 0,  0>
look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <0.75, 0.25, 0.25>}
light_source {<100, 100, 100> color rgb <0.0, 0.4, 0.75>}


// row 1

sphere { <-4, -4, -2>, 1.5
pigment { color rgb <1.0, 0.0, 1.0>}
finish {ambient 0.1 diffuse 0.9 specular 0.1 roughness 0.1 reflection 0.5}
}



sphere { <0, -4, 0>, 1.5
pigment { color rgb <1.0, 1.0, 1.0, 0.5>}
finish {ambient 0.1 diffuse 0.9 refraction 1.0 ior 1.6}
}



sphere { <4, -4, -2>, 1.5
pigment { color rgb <0.0, 1.0, 1.0>}
finish {ambient 0.1 diffuse 0.9 specular 0.1 roughness 0.5 reflection 0.5}
}


// row 2



// row 3
sphere { <-2, 0, -3>, 1.8
pigment { color rgb <0.0, 1.0, 0.0>}
finish {ambient 0.2 diffuse 0.5 specular 0.5 roughness 0.2 reflection 0.4}
}



sphere { <2, 0, 1>, 0.8
pigment { color rgb <1.0, 1.0, 0.0>}
finish {ambient 0.1 diffuse 0.5 specular 0.5 roughness 0.4}
}



// row 4


// row 5

sphere { <-5, 2, 0>, 1.0
pigment { color rgb <1.0, 0.0, 0.0>}
finish {ambient 0.4 diffuse 0.1 specular 0.9 roughness 0.1 reflection 0.9}
}


sphere { <1, 4, 0>, 0.8
pigment { color rgb <0.0, 0.0, 1.0, 0.5>}
finish {ambient 0.2 diffuse 0.4 specular 0.9 roughness 0.3 refraction 1.0 ior 1.6}
}


sphere { <4, 3, 0>, 1.2
pigment { color rgb <1.0, 1.0, 1.0>}
finish {ambient 0.1 diffuse 0.1 specular 0.9 roughness 0.5 reflection 0.2}
}


// plane

plane {<0, 1, 0>, -6
pigment {color rgb <0.8, 0.5, 1.0>}
finish {ambient 0.4 diffuse 0.8 reflection 0.7}
}