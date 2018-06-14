camera {
location  <0, 0, 14>
up        <0,  1,  0>
right     <1.33333, 0,  0>
look_at   <0, 0, 0>
}

light_source {<-50, 50, 50> color rgb <1.5, 1.5, 1.5>}

//closest
sphere { <-2, 0, 10>, 1
pigment { color rgb <1.0, 1.0, 1.0>}
finish {ambient 0.2 diffuse 0.4}
}

//middle
sphere { <0, 0, 8>, 1
pigment { color rgb <1.0, 1.0, 1.0>}
finish {ambient 0.2 diffuse 0.4}
}

//farther
sphere { <2, 0, 6>, 1
pigment { color rgb <1.0, 1.0, 1.0>}
finish {ambient 0.2 diffuse 0.4}
}

//farthest
sphere { <4, 0, 4>, 1
pigment { color rgb <1.0, 1.0, 1.0>}
finish {ambient 0.2 diffuse 0.4}
}

plane {<0, 1, 0>, -4
pigment {color rgb <0.6, 0.2, 1.0>}
finish {ambient 0.3 diffuse 0.8 specular 0.8 roughness 0.001 reflection 0.5}
}