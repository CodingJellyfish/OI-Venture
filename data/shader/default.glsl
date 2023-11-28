@vs vs
in vec4 coord;
in vec4 color;
out vec2 texcoord;
out vec4 vert_color;
void main() {
    gl_Position = vec4(coord.xy, 0.0, 1.0);
    texcoord = coord.zw;
    vert_color = color;
}
@end

@fs fs
uniform texture2D tex;
uniform sampler smp;
in vec2 texcoord;
in vec4 vert_color;
out vec4 frag_color;
void main() {
    frag_color = texture(sampler2D(tex, smp), texcoord) * vert_color;
}
@end

@program default2d vs fs