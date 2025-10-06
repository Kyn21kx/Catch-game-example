const std = @import("std");
const zcc = @import("compile_commands");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});

    const exe = b.addExecutable(.{ .name = "Game", .target = target });

    var targets = std.ArrayList(*std.Build.Step.Compile).init(b.allocator);
    targets.append(exe) catch @panic("OOM");

    exe.addCSourceFile(.{ .file = b.path("src/main.cpp"), .flags = &[_][]const u8{"-std=c++20"}, .language = .cpp });

    zcc.createStep(b, "cdb", targets.toOwnedSlice() catch @panic("OOM"));

    exe.addIncludePath(b.path("third_party/raylib/include/"));

    exe.linkSystemLibrary("raylib");
    exe.addLibraryPath(b.path("third_party/raylib/lib"));
    exe.linkLibCpp();

    b.installArtifact(exe);
}
