// batch.hpp

#pragma once

#include <initializer_list>
#include <SFML/Graphics.hpp>

namespace runner
{
   class PrimitiveBatch final
   {
      static constexpr int default_vertex_count = 16 * 1024;

   public:
      PrimitiveBatch();
      PrimitiveBatch(std::size_t triangle_capacity);

      void clear();
      void draw_line(const sf::Vector2f &from, const sf::Vector2f &to, const float thickness, const sf::Color &color);
      void draw_line(const sf::Vector2f &from, const sf::Vector2f &to, const float thickness, const sf::Color &from_color, const sf::Color &to_color);
      void draw_circle(const sf::Vector2f &center, const float radius, const int sections, const sf::Color &color);
      void draw_circle(const sf::Vector2f &center, const float radius, const int sections, const float thickness, const sf::Color &color);
      void draw_rectangle(const sf::FloatRect &rect, const sf::Color &color);
      void draw_rectangle(const sf::FloatRect &rect, const float thickness, const sf::Color &color);
      void present(sf::RenderTarget &target);

   private:
      const std::size_t m_vertex_capacity = 0; //TODO: const member
      sf::VertexArray   m_vertex_array;
      sf::VertexBuffer  m_vertex_buffer;
   };
} // !runner
