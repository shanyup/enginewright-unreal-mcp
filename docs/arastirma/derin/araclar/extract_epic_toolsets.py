import re, json, os, sys, glob
ROOT = sys.argv[1]  # Engine/Plugins/Experimental
out = {}

def first_line(doc):
    doc = (doc or '').strip().strip('"').strip()
    return doc.split('\n')[0].strip()[:160]

# C++: headers with class X : public UToolsetDefinition (or MCPClient variants)
for h in glob.glob(os.path.join(ROOT, '**', '*.h'), recursive=True):
    if 'Intermediate' in h or os.sep + 'Tests' + os.sep in h or 'Mock' in h:
        continue
    src = open(h, encoding='utf-8', errors='ignore').read()
    for m in re.finditer(r'class\s+(?:\w+_API\s+)?(U\w+)\s*:\s*public\s+(U\w*Toolset\w*|UToolsetDefinition)', src):
        cls = m.group(1)
        start = m.end()
        nxt = re.search(r'\nUCLASS\(', src[start:])
        body = src[start: start + nxt.start()] if nxt else src[start:]
        tools = []
        for fm in re.finditer(r'((?:[ \t]*/\*\*.*?\*/\s*)?)UFUNCTION\(([^\n]*)\)\s*\n\s*(?:static\s+)?([^\n;(]+?)\s+(\w+)\s*\(', body, re.S):
            doc = re.sub(r'[/*\t]+', ' ', fm.group(1))
            doc = re.sub(r'\s+', ' ', doc).strip()
            tools.append({'name': fm.group(4), 'doc': doc[:200], 'meta': fm.group(2)[:120]})
        if tools:
            key = os.path.relpath(h, ROOT).split(os.sep)
            out[f"{key[1] if key[0]=='Toolsets' else key[0]}/{cls}"] = {'lang': 'C++', 'file': os.path.relpath(h, ROOT), 'tools': tools}

# Python
for p in glob.glob(os.path.join(ROOT, '**', '*.py'), recursive=True):
    if 'tests' in p.split(os.sep) or '__pycache__' in p:
        continue
    src = open(p, encoding='utf-8', errors='ignore').read()
    if 'ToolsetDefinition)' not in src:
        continue
    for cm in re.finditer(r'class\s+(\w+)\((?:unreal\.)?ToolsetDefinition\):', src):
        start = cm.end()
        nxt = re.search(r'\n(?:@unreal\.uclass\(\)\s*\n)?class\s', src[start:])
        body = src[start: start + nxt.start()] if nxt else src[start:]
        tools = []
        for fm in re.finditer(r'@(?:\w+\.)?tool_call[^\n]*\n(?:\s*@[^\n]*\n)*\s*(?:async\s+)?def\s+(\w+)\s*\(', body):
            rest = body[fm.end():fm.end() + 4000]
            dm = re.search(r'"""(.*?)"""', rest, re.S)
            nd = re.search(r'\n\s*(?:@|def )', rest)
            doc = dm.group(1) if dm and (not nd or dm.start() < nd.start()) else ''
            tools.append({'name': fm.group(1), 'doc': first_line(doc)})
        if tools:
            parts = os.path.relpath(p, ROOT).split(os.sep)
            out[f"{parts[1] if parts[0]=='Toolsets' else parts[0]}/{cm.group(1)}"] = {'lang': 'Python', 'file': os.path.relpath(p, ROOT), 'tools': tools}

json.dump(out, open(sys.argv[2], 'w', encoding='utf-8'), indent=1, ensure_ascii=False)
print(len(out), sum(len(v['tools']) for v in out.values()))
