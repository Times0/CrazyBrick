import pygame
from pygame import Color


def distance_point_to_segment(x, y, x1, y1, x2, y2) -> float:
    # calculate the length of the segment
    L = ((x1 - x2) ** 2 + (y1 - y2) ** 2) ** 0.5

    # if the segment is a point
    if L == 0:
        return ((x - x1) ** 2 + (y - y1) ** 2) ** 0.5

    # calculate the projection of the point on the segment
    t = ((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) / L**2

    # if the projection is outside the segment
    if t < 0:
        return ((x - x1) ** 2 + (y - y1) ** 2) ** 0.5

    if t > 1:
        return ((x - x2) ** 2 + (y - y2) ** 2) ** 0.5

    # calculate the projection point
    xp = x1 + t * (x2 - x1)
    yp = y1 + t * (y2 - y1)

    # calculate the distance between the point and the projection point
    return ((x - xp) ** 2 + (y - yp) ** 2) ** 0.5


import math
from pygame import Vector2


center = (400, 300)


def get_unit_cirlce_pos(i, n, r, center) -> Vector2:
    angle = 2 * math.pi * i / n
    x1, y1 = math.cos(angle) * r + center[0], math.sin(angle) * r + center[1]
    return Vector2(x1, y1)


def generate_coords(n, h) -> list[tuple[Vector2, Vector2, Vector2, Vector2]]:
    coords: list[tuple[Vector2, Vector2, Vector2, Vector2]] = []

    for r in range(100, 300, h):
        for i in range(n):
            p1 = get_unit_cirlce_pos(i, n, r, center)
            p2 = get_unit_cirlce_pos(i + 1, n, r, center)
            p3 = get_unit_cirlce_pos(i + 1, n, r + h, center)
            p4 = get_unit_cirlce_pos(i, n, r + h, center)
            coords.append((p1, p2, p3, p4))
    return coords


if __name__ == "__main__":
    pygame.init()
    screen = pygame.display.set_mode((800, 600))
    running = True

    font = pygame.font.Font(None, 36)
    polygon_points = [(100, 100), (200, 200), (300, 100), (200, 50)]
    radius = 50

    points = generate_coords(20, 20)

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
        screen.fill((0, 0, 0))
        pygame.draw.polygon(screen, Color("white"), polygon_points, width=1)
        mouse_x, mouse_y = pygame.mouse.get_pos()

        for i in range(len(polygon_points)):
            x1, y1 = polygon_points[i]
            x2, y2 = polygon_points[(i + 1) % len(polygon_points)]
            distance = distance_point_to_segment(mouse_x, mouse_y, x1, y1, x2, y2)
            if distance < radius:
                break

        color = "green" if distance < radius else "red"

        pygame.draw.circle(screen, Color(color), (mouse_x, mouse_y), radius, width=1)
        # draw distance
        text = font.render(f"{distance:.2f}", True, (255, 255, 255))
        screen.blit(text, (mouse_x + 10, mouse_y + 10))

        # draw bricks
        for p1, p2, p3, p4 in points:
            pygame.draw.polygon(screen, Color("white"), [(p1.x, p1.y), (p2.x, p2.y), (p3.x, p3.y), (p4.x, p4.y)], width=1)

        for p1, p2, p3, p4 in points:
            pygame.draw.circle(screen, Color("red"), (int(p1.x), int(p1.y)), 5, width=1)
            pygame.draw.circle(screen, Color("red"), (int(p2.x), int(p2.y)), 5, width=1)
            pygame.draw.circle(screen, Color("red"), (int(p3.x), int(p3.y)), 5, width=1)
            pygame.draw.circle(screen, Color("red"), (int(p4.x), int(p4.y)), 5, width=1)

        pygame.display.flip()
    pygame.quit()
