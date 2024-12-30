#pragma once

namespace CRTShader {

constexpr char fragment_shader[] = R"(
#version 330 core
in vec2 texCoord;
out vec4 fragColor;
uniform sampler2D screenTexture;

void main() {
    // Basic screen curvature
    vec2 uv = texCoord * 2.0 - 1.0;
    vec2 offset = abs(uv.yx) / vec2(3.0, 6.0);
    uv = uv + uv * offset * offset;
    uv = uv * 0.5 + 0.5;

    // Skip if outside bounds
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    // Simple RGB shift
    vec3 col;
    col.r = texture(screenTexture, uv + vec2(0.001, 0.0)).r;
    col.g = texture(screenTexture, uv).g;
    col.b = texture(screenTexture, uv - vec2(0.001, 0.0)).b;

    // Basic scanlines
    float scanline = sin(uv.y * 800.0) * 0.02;
    col -= scanline;

    // Vignette
    float vignette = uv.x * uv.y * (1.0 - uv.x) * (1.0 - uv.y);
    vignette = pow(vignette * 15.0, 0.25);
    col *= vignette;

    fragColor = vec4(col, 1.0);
}
)";

} // namespace CRTShader