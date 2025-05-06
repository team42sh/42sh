# -------- Stage 1: Build ------------
FROM ubuntu:22.04 as builder

# Avoid prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y build-essential make

# Clean up apt cache (Free space)
RUN apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /build

COPY . .

RUN ./scripts/build/1-prepare-docker.sh
RUN ./scripts/build/2-build.sh

# -------- Stage 2: Final Image ------------
FROM ubuntu:22.04

RUN apt-get update

# Clean up apt cache (Free space)
RUN apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /build/42sh /usr/bin/42sh
RUN chmod +x /usr/bin/42sh

ENTRYPOINT ["/usr/bin/42sh"]
