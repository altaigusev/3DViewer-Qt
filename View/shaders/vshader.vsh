#extension GL_EXT_gpu_shader4 : require
attribute highp vec4 a_position;
attribute highp vec2 a_texcoord;
attribute highp vec3 a_normal;
uniform highp mat4 u_projectionMatrix; // матрица проекций
uniform highp mat4 u_viewMatrix; // видовая (камер) матрица
uniform highp mat4 u_modelMatrix; // модельная матрица
varying highp vec4 v_position;
varying highp vec2 v_texcoord;
flat varying vec3 v_normal;
varying vec3 v_normal2;
// a_ переменные типа атрибут
// v_ переменные типа вариан
// u_ переменные типа униформ
void main(void)
{
    mat4 mv_matrix = u_viewMatrix * u_modelMatrix; // модельновидовая матрица
    gl_Position = u_projectionMatrix * mv_matrix * a_position; // формирование конечных координат вершин
    v_texcoord = a_texcoord;
    v_normal = normalize(vec3(mv_matrix * vec4(a_normal, 0.0)));
    v_normal2 = normalize(vec3(mv_matrix * vec4(a_normal, 0.0)));
    v_position = mv_matrix * a_position;
}
