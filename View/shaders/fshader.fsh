#extension GL_EXT_gpu_shader4 : require
uniform sampler2D u_texture;
uniform highp bool shading;
uniform highp vec4 u_colorModel; // использовать в резултколор
uniform highp vec4 u_lightPosition;
uniform highp float u_lightPower; // сила освещения
uniform highp float u_colorRed;
uniform highp float u_colorGreen;
uniform highp float u_colorBlue;
varying highp vec4 v_position;
varying highp vec2 v_texcoord;
flat varying vec3 v_normal;
varying vec3 v_normal2;
void main(void)
{
    vec3 final_normal = shading ? v_normal : v_normal2;
    vec4 resultColor = u_colorModel; // результирующий цвет чёрный
    vec4 eyePosition = vec4(0.0, 0.0, 0.0, 1.0); // позиция наблюдателя
    vec4 diffMatColor = texture2D(u_texture, v_texcoord); // диффузный цвет материала
    vec3 eyeVec = normalize(v_position.xyz - eyePosition.xyz); // направление взгляда
    vec3 lightVec = normalize(v_position.xyz - u_lightPosition.xyz); // вектор освещения
    vec3 reflectLight = normalize(reflect(lightVec, final_normal));;
    float len = length(v_position.xyz - eyePosition.xyz); // расстояние от наблюдателя до точки
    float specularFactor = 60.0; // размер пятна блика
    float ambientFactor = 0.3; // светимость материала
    vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(final_normal, -lightVec) / (1.0 + 0.25 * pow(len, 2.0)));
    resultColor += diffColor;
    vec4 ambientColor = ambientFactor * diffMatColor;
    resultColor += ambientColor;
    vec4 specularColor = vec4(u_colorRed, u_colorGreen, u_colorBlue, 1.0) * u_lightPower * pow(max(0.0, dot(reflectLight, -eyeVec)), specularFactor) / (1.0 + 0.25 * pow(len, 2.0));
    resultColor += specularColor;
    resultColor *= vec4(u_colorRed, u_colorGreen, u_colorBlue, 1.0);
    gl_FragColor = resultColor;
}
