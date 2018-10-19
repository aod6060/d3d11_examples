#pragma once



void rend_init();
void rend_release();

void rend_clear(const glm::vec4& color, float depth);
void rend_present();

ID3D11Device* rend_getDevice();
ID3D11DeviceContext* rend_getContext();
