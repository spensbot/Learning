// Jest setup file for Asteroids game tests

// Mock Phaser for testing
(global as any).Phaser = {
    Scene: class MockScene {},
    Game: class MockGame {},
    Physics: {
        Arcade: {
            Body: class MockBody {}
        }
    },
    GameObjects: {
        Sprite: class MockSprite {},
        Graphics: class MockGraphics {}
    },
    Input: {
        Keyboard: {
            Key: class MockKey {}
        }
    }
} as any;

// Mock DOM methods that might be used in tests
Object.defineProperty(window, 'requestAnimationFrame', {
    writable: true,
    value: (callback: FrameRequestCallback) => {
        return setTimeout(callback, 16);
    }
});

Object.defineProperty(window, 'cancelAnimationFrame', {
    writable: true,
    value: (id: number) => {
        clearTimeout(id);
    }
});