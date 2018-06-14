camera {
location  <0, 0, 14>
up        <0,  1,  0>
right     <1.33333, 0,  0>
look_at   <0, 0, 0>
}

light_source {<50, 50, 50> color rgb <2.0, 2.0, 2.0>}



//front
sphere { <0, -1, 8.5>, 1
pigment { color rgb <0.7, 0.7, 1.0>}
finish {ambient 0.2 diffuse 0.4}
}

//middle right
sphere { <3.5, -1, 2>, 1.5
pigment { color rgb <0.9, 1.0, 0.9>}
finish {ambient 0.2 diffuse 0.4}
}

//back right
sphere { <6, 0.5, 0>, 1.5
pigment { color rgb <0.6, 1.0, 0.6>}
finish {ambient 0.2 diffuse 0.4}
}

//middle left
sphere { <-3.5, -1, 2>, 1.5
pigment { color rgb <1.0, 0.9, 0.9>}
finish {ambient 0.2 diffuse 0.4}
}

//back left
sphere { <-6, 0.5, 0>, 1.5
pigment { color rgb <1.0, 0.6, 0.6>}
finish {ambient 0.2 diffuse 0.4}
}

//back middle right
sphere { <3.25, 2, -2>, 1.5
pigment { color rgb <0.4, 1.0, 0.4>}
finish {ambient 0.2 diffuse 0.4}
}

//back middle left
sphere { <-3.25, 2, -2>, 1.5
pigment { color rgb <1.0, 0.4, 0.4>}
finish {ambient 0.2 diffuse 0.4}
}

//back
sphere { <0, 3.5, -3.5>, 1
pigment { color rgb <0.5, 0.5, 1.0>}
finish {ambient 0.2 diffuse 0.4}
}




plane {<0, 1, 0>, -4
pigment {color rgb <0.9, 0.4, 1.0>}
finish {ambient 0.3 diffuse 0.8 specular 0.8 roughness 0.001 reflection 0.8}
}