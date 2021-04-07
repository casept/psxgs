/*
 * This sample demonstrates how to initialize the library
 * and draw a very simple TMD file
 * consisting of a single 3D object which is just an untextured triangle.
 */

#include <psxgpu.h>
#include <psxgs.h>
#include <stdbool.h>

// Include the data by using the GAS .incbin directive.
// In a real game, you'd probably want to load from disk instead.
extern __attribute__((aligned(16))) unsigned long Triangle_tmd[];

#define MAX_NUM_PACKETS 1024
// A GPU packet can be up to 24 bytes in size.
#define MAX_PACKET_SIZE 24
// The PACKET type is just an alias for an unsigned char, as GPU packet size varies.
#define PACKET_AREA_SIZE MAX_NUM_PACKETS* MAX_PACKET_SIZE

// The library generates GPU primitive command packets and needs
// a place to store them. This is the space where that happens.
// There are 2 packet buffers to enable processing the next frame on the CPU+GTE while the GPU is still drawing.
PACKET Gpu_packets[2][PACKET_AREA_SIZE];

// The ordering table is where references to GPU packets in the GPU packet buffer are stored.
// This is just a handle to the table, the actual table is defined below.
// Once again, there are 2 to enable double buffering.
GsOT OT_handles[2];

// The maximum Z drawing precision is determined by the ordering table's size.
// The library chops off as many bits as needed from the Z coordinate of a primitive to make it fit in the table.
// This value describes how many bits to keep.
// The ordering table itself must be exactly 2^OT_PRECISION in size.
#define OT_PRECISION 10
#define OT_SIZE      1024  // 2^10

// These are the actual table entries. They make up a linked list telling the GPU which primitives to draw when.
GsOT_TAG OTs[2][OT_SIZE];

// Each 2D or 3D object instantiated by libgs can be interacted with using a handle. This is the handle to our triangle.
GsDOBJ2 Triangle_handle;

// All the tasks that typically have to be performed only once on program start.
void init_graphics() {
    // This sample assumes an NTSC machine.
    // However, customizing it for PAL is just a matter of changing the resolution
    // where needed.
    SetVideoMode(MODE_NTSC);
    // Initialize the library's core data structures and the hardware.
    GsInitGraph(640, 480, GsINTER | GsOFSGPU | GsRESET0, GsDITHER, GsVRAM16BIT);
    // Configure the coordinates of double buffers in VRAM.
    // With this setup, the buffer resides in the upper-left column of VRAM.
    GsDefDispBuff(0, 0, 0, 480);
    // Initialize data structures needed for 3D functionality,
    // such as the default light matrix.
    // Also initializes the GTE.
    GsInit3D();

    // Link the OT handles with the actual space where the primitive references are stored.
    OT_handles[0].length = OT_PRECISION;
    OT_handles[0].org = OTs[0];
    OT_handles[1].length = OT_PRECISION;
    OT_handles[1].org = OTs[1];

    // Remap the addresses in the TMD to point to where it actually is in process memory.
    GsMapModelingData(Triangle_tmd);
    // Link the triangle in the TMD to it's handle so we can manipulate and sort it.
    // The unsigned long argument here is indeed a pointer, but Sony decided for whatever reason to pass it as an
    // integer.
    GsLinkObject4((unsigned long)Triangle_tmd, &Triangle_handle, 0);
}

int main(void) {
    init_graphics();

    // Do everything that has to be done each frame (at least in a real game where the scene actually changes).
    while (true) {
        // Figure out which of the buffers is currently the back (AKA drawing) buffer.
        const int back_buffer = GsGetActiveBuffer();
        // Tell the library to put new GPU primitives into the back packet buffer.
        GsSetWorkBase(Gpu_packets[back_buffer]);
        // Clear the ordering table so that new primitives can be added.
        GsClearOT(0, 0, &OT_handles[back_buffer]);
        // Perform perspective transformations and sort the triangle to the OT.
        // FIXME: Currently, the scratchpad is not used. Fix address once it is.
        GsSortObject4(&Triangle_handle, &OT_handles[back_buffer], OT_PRECISION, 0);

        // Wait for the GPU to finish drawing.
        DrawSync(0);
        // Wait for VSync.
        VSync(0);

        // Swap the front and back buffers.
        GsSwapDispBuffer();
        // Sort a packet ordering the GPU to clear the screen before doing anything else.
        GsSortClear(0x0, 0x0, 0x0, &OT_handles[back_buffer]);
        // Start drawing the OT to the back buffer. This function runs asynchronously, meaning that it returns before
        // drawing is actually done.
        GsDrawOT(&OT_handles[back_buffer]);
    }
    // The program should never terminate, this is just here to make the compiler happy.
    return 0;
}
