import * as THREE from 'https://cdn.jsdelivr.net/npm/three@0.158.0/build/three.module.js';
import { GLTFExporter } from 'https://cdn.jsdelivr.net/npm/three@0.158.0/examples/jsm/exporters/GLTFExporter.js';
import { FontLoader } from 'https://cdn.jsdelivr.net/npm/three@0.158.0/examples/jsm/loaders/FontLoader.js';
import { TextGeometry } from 'https://cdn.jsdelivr.net/npm/three@0.158.0/examples/jsm/geometries/TextGeometry.js';
import { saveAs } from 'https://cdn.jsdelivr.net/npm/file-saver@2.0.5/dist/FileSaver.min.js';

document.getElementById('exportBtn').addEventListener('click', () => {
    const scene = new THREE.Scene();

    const loader = new FontLoader();
    loader.load('https://threejs.org/examples/fonts/helvetiker_regular.typeface.json', function (font) {
        const textGeometry = new TextGeometry('Welcome to the CAP project', {
            font: font,
            size: 1,
            height: 0.2,
            curveSegments: 12,
            bevelEnabled: false
        });

        const material = new THREE.MeshStandardMaterial({ color: 0x00ff00 });
        const textMesh = new THREE.Mesh(textGeometry, material);
        scene.add(textMesh);

        const light = new THREE.DirectionalLight(0xffffff, 1);
        light.position.set(5, 5, 5);
        scene.add(light);

        const exporter = new GLTFExporter();
        exporter.parse(scene, function(result) {
            if (result instanceof ArrayBuffer) {
                const blob = new Blob([result], { type: 'application/octet-stream' });
                saveAs(blob, 'text_model.glb');
            }
        }, { binary: true });
    });
});

