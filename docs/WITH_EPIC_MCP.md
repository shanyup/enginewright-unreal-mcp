# Using Enginewright together with Epic's MCP server

UE 5.8 ships its own experimental MCP server (the **Unreal MCP** plugin, `ModelContextProtocol`, with Epic's toolsets).
Enginewright does not need it, does not wrap it and does not copy its tools. The two can run in the same editor at the
same time, and one agent can use both: Epic's toolsets for the wide editor surface, Enginewright for running the game,
multiplayer tests, game input, audits and checked writes.

Tested on UE 5.8 Win64 with Claude Code.

## 1. Two servers, two ports

| Server | Default address | Auth |
|---|---|---|
| Enginewright | `http://127.0.0.1:3100/mcp` | per-project token (`X-Enginewright-Token`) |
| Epic Unreal MCP | `http://127.0.0.1:8000/mcp` | none (localhost only) |

They never share a port. If you change one, keep them apart: Enginewright's port is in its panel (or
`ENGINEWRIGHT_PORT`), Epic's is `ServerPortNumber` in its settings (or `-ModelContextProtocolPort=N`).

## 2. Turn on Epic's server

1. Edit > Plugins: enable **Unreal MCP** and the toolsets you want (for example **All Toolsets**). Restart the editor.
2. Start the server, one of:
   - Project Settings, page **Model Context Protocol** > Server > **Auto Start Server** (saved per user and project);
   - start the editor with `-ModelContextProtocolStartServer`;
   - console command `ModelContextProtocol.StartServer` (once per editor session).
3. The Output Log shows `Starting MCP server on port 8000`.

Enginewright starts on its own; its Output Log line is `Enginewright: MCP server on http://127.0.0.1:3100/mcp`.

## 3. Connect the agent to both

Claude Code, in your project folder:

```
claude mcp add --transport http unreal-mcp http://127.0.0.1:8000/mcp
```

and the Enginewright command from `<YourProject>/Saved/Enginewright/clients/claude-code.txt` (it carries the token).
`claude mcp list` should show both as connected.

Other clients: add Epic's server as a second HTTP entry next to the Enginewright entry from the same `clients/` folder.

## 4. Which one to ask

Each server shows the agent a small set of meta tools: Enginewright `search` / `describe` / `call`, Epic
`list_toolsets` / `describe_toolset` / `call_tool` (with tool search on, the default). Tell the agent which server owns
which job, for example in your `CLAUDE.md` / `AGENTS.md`:

- Run the game, Play In Editor with several players, game input, screenshots and image checks, localization checks,
  audits, writes that must be read back: **Enginewright**.
- Editor areas Enginewright does not cover (see [SUPPORT.md](SUPPORT.md)): **Epic's toolsets**.

## Notes

- Both servers accept requests only from this computer. Enginewright also refuses browser requests from other web
  sites (the `Origin` header); trusted local web tools can be added under Allowed Origins in its settings.
- Enginewright's read-only mode and disabled categories apply to Enginewright only, not to Epic's server.
- Epic's plugin is experimental in 5.8. Its tools and settings may change between engine versions.
