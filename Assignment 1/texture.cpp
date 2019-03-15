#include "texture.hpp"

Texture::Texture(std::vector<Eigen::MatrixXd>& img) : id(txt_count++), texture(img) {
    height = texture[0].rows();
    width = texture[0].cols();
}