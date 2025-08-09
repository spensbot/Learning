export interface Rectangle {
    x: number;
    y: number;
    width: number;
    height: number;
}

export interface ICollidable {
    getCollisionBounds(): Rectangle;
    onCollision(other: ICollidable): void;
}