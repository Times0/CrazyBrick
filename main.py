import pygame
from pygame import Color


def distance_point_to_segment(x, y, x1, y1, x2, y2) -> float:
    # calculate the length of the segment
    L = ((x1 - x2) ** 2 + (y1 - y2) ** 2) ** 0.5

    # if the segment is a point
    if L == 0:
        return ((x - x1) ** 2 + (y - y1) ** 2) ** 0.5

    # calculate the projection of the point on the segment
    t = ((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) / L ** 2

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


if __name__ == '__main__':
    pygame.init()
    screen = pygame.display.set_mode((800, 600))
    running = True

    font = pygame.font.Font(None, 36)

    polygon_points = [(100, 100), (200, 200), (300, 100), (200, 50)]

    radius = 50

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
        text = font.render(f'{distance:.2f}', True, (255, 255, 255))
        screen.blit(text, (mouse_x + 10, mouse_y + 10))

        pygame.display.flip()
    pygame.quit()
