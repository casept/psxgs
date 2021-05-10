/*
 * This sample demonstrates how to initialize the library
 * and draw a very simple TMD file
 * consisting of a single 3D object which is just an untextured cube.
 */

#include <psxgpu.h>
#include <psxgs.h>
#include <stdbool.h>
#include <stdio.h>

// Include the data by using the GAS .incbin directive.
// In a real game, you'd probably want to load from disk instead.
extern __attribute__((aligned(16))) unsigned long Cube_Tmd[];

#define MAX_NUM_PACKETS 1024
// A GPU packet can be up to 24 bytes in size.
#define MAX_PACKET_SIZE 24
// The PACKET type is just an alias for an unsigned char, as GPU packet size varies.
#define PACKET_AREA_SIZE MAX_NUM_PACKETS* MAX_PACKET_SIZE

// The library generates GPU primitive command packets and needs
// a place to store them. This is the space where that happens.
// There are 2 packet buffers to enable processing the next frame on the CPU+GTE while the GPU is still drawing.
PACKET __attribute__((aligned(32))) Gpu_packets[2][PACKET_AREA_SIZE];

// The ordering table is where references to GPU packets in the GPU packet buffer are stored.
// This is just a handle to the table, the actual table is defined below.
// Once again, there are 2 to enable double buffering.
GsOT OT_handles[2];

// The maximum Z drawing precision is determined by the ordering table's size.
// The library chops off as many bits as needed from the Z coordinate of a primitive to make it fit in the table.
// This value describes how many bits to keep.
// The ordering table itself must be exactly 2^OT_PRECISION in size.
#define OT_PRECISION     10
#define OT_SIZE          1024  // 2^10
#define MAX_OT_PRECISION 14

#define NTSC_WIDTH  320
#define NTSC_HEIGHT 240

// These are the actual table entries. They make up a linked list telling the GPU which primitives to draw when.
GsOT_TAG OTs[2][OT_SIZE];

// Each 2D or 3D object instantiated by libgs can be interacted with using a handle. This is the handle to our cube.
GsDOBJ2 Cube_Handle;

// This is a handle for the libgs viewpoint/camera.
GsVIEW2 View = {{{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {0, 0, 0}}, &WORLD};

// All the tasks that typically have to be performed only once on program start.
void init_graphics(void) {
    // This sample assumes an NTSC machine.
    // However, customizing it for PAL is just a matter of changing the resolution
    // where needed.
    SetVideoMode(MODE_NTSC);
    // Initialize the library's core data structures and the hardware.
    GsInitGraph(NTSC_WIDTH, NTSC_HEIGHT, GsNONINTER | GsOFSGPU | GsRESET0, GsDITHER, GsVRAM16BIT);
    // Configure the coordinates of double buffers in VRAM.
    // With this setup, the buffers reside in the upper-left column of VRAM.
    GsDefDispBuff(0, 0, 0, NTSC_HEIGHT);
    // Initialize data structures needed for 3D functionality,
    // such as the default light matrix.
    // Also initializes the GTE.
    GsInit3D();

    // Link the OT handles with the actual tables.
    OT_handles[0].length = OT_PRECISION;
    OT_handles[0].org = OTs[0];
    OT_handles[1].length = OT_PRECISION;
    OT_handles[1].org = OTs[1];

    // The view's superior coordinates are world coordinates.
    View.super = &WORLD;
    // Register it as the default viewpoint.
    GsSetView2(&View);

    // Set the distance between the surface the 3D geometry is projected onto and the observer.
    // This effectively sets the FOV.
    GsSetProjection(1000);

    // Remap the addresses in the TMD to point to where it actually is in process memory.
    GsMapModelingData(Cube_Tmd);
    // Link the cube in the TMD to it's handle so we can manipulate and sort it.
    // The unsigned long argument here is indeed a pointer, but Sony decided for whatever reason to pass it as an
    // integer.
    GsLinkObject4((unsigned long)Cube_Tmd, &Cube_Handle, 0);
}

// Moves the camera based on controller input.
void move_camera() {
    // TODO: Read controller
    View.view.m[0][0] += 1;
    View.view.m[0][1] += 1;
    View.view.m[0][2] += 1;
    View.view.m[1][0] += 1;
    View.view.m[1][1] += 1;
    View.view.m[1][2] += 1;
    View.view.m[2][0] += 1;
    View.view.m[2][1] += 1;
    View.view.m[2][2] += 1;
    // TODO: Set rotation
    // Resetting this is technically not needed each frame
    // if the parent coordinate system is static, but it's probably a good idea
    // to do it always so one doesn't forget when it actually is needed.
    GsSetView2(&View);
}

int main(void) {
    init_graphics();

    // Do everything that has to be done each frame (at least in a real game where the scene actually changes).
    while (true) {
        // Swap the front and back buffers.
        GsSwapDispBuffer();

        // Figure out which of the buffers is currently the back (AKA drawing) buffer.
        const int back_buffer = GsGetActiveBuffer();
        // Tell the library to put new GPU primitives into the back packet buffer.
        GsSetWorkBase(Gpu_packets[back_buffer]);
        // Clear the ordering table so that new primitives can be added.
        GsClearOT(0, 0, &OT_handles[back_buffer]);
        // Sort a packet ordering the GPU to clear the screen before doing anything else.
        GsSortClear(0, 0, 0, &OT_handles[back_buffer]);

        // Perform perspective transformations and sort the cube to the OT.
        // FIXME: Currently, the scratchpad is not used. Fix address once it is.
        GsSortObject4(&Cube_Handle, &OT_handles[back_buffer], MAX_OT_PRECISION - OT_PRECISION, 0);

        // Start drawing the OT to the back buffer. This function runs asynchronously, meaning that it returns before
        // drawing is actually done.
        GsDrawOT(&OT_handles[back_buffer]);

        // Change camera position for the next frame.
        move_camera();

        // This is where you would usually perform non-graphics game logic while letting the GPU work in the background.
        // Since we don't have any, we simply wait on the drawing to finish and a new frame to begin.
        DrawSync(0);
        VSync(0);
    }
    // The program should never terminate, this is just here to make the compiler happy.
    return 0;
}
