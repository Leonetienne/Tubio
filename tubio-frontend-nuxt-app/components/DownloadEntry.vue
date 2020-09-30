<template>
    <div class="frame mb-6 pt-2 pb-4">

        <div class="flex flex-col">

            <div class="flex items-end justify-between w-full md:w-auto">

              <div class="flex flex-col">
                <div class="icon--mode">
                  <IconFilm v-if="downloadEntry.mode === 'video'" />
                  <IconMusic v-else />
                </div>
                <div class="timestamp">
                    {{getQueuedDateString(downloadEntry.queued_timestamp)}}
                </div>
              </div>

                <div class="button-remove" v-on:click="removeDownload">
                    <IconX />
                </div>
            </div>

            <div class="flex flex-col-reverse md:flex-row w-full mt-2">
            
                <div class="flex flex-col">
                    <a :href="downloadEntry.webpage_url" target="_blank" title="To the original source">
                        <div class="thumbnail flex-shrink-0" :style="'--thumbnail: url(\'' + downloadEntry.thumbnail_url + '\')'">
                            <div class="thumbnail__vignette" />
                            <div class="thumbnail__duration">{{getDurationString(downloadEntry.duration)}}</div>
                        </div>
                    </a>

                    <!-- Progressbar -->
                    <div v-if="downloadEntry.status === 'downloading'">
                        <div class="status--progressbar flex w-full mt-3">
                            <div class="status--progressbar__good items-stretch" :style="'--download-progress: ' + downloadEntry.download_progress + '%;'"></div>
                        </div>
                        <div class="status--progressbar__text">
                            {{downloadEntry.download_progress}}%
                        </div>
                    </div>

                    <!-- Ready -->
                    <a v-else-if="downloadEntry.status === 'finished'" :href="downloadEntry.download_url" title="download">
                        <div class="status--ready mt-3 button flex justify-center w-full">
                            <div>
                                <IconDownload />
                            </div>
                        </div>
                    </a>

                     <!-- Queued -->
                    <div v-else-if="downloadEntry.status === 'queued'">
                        <div class="status--queued mt-3">
                            Queued
                        </div>
                    </div>

                    <!-- Failed -->
                    <div v-else-if="downloadEntry.status === 'failed'">
                        <div class="status--failed mt-3">
                            Failed!
                        </div>
                    </div>

                </div>

                <div class="flex flex-col md:ml-4 w-full overflow-x-hidden overflow-y-visible">
                    <h1 class="title">{{downloadEntry.title}}</h1>
                    
                    <div class="relative my-4">
                      <div v-if="downloadEntry.description != ''">
                        <p class="description p-2">
                            <span v-html="linebreaksToBrTags(downloadEntry.description)" />
                        </p>
                        <div class="description__decobox description__decobox--left" />
                        <div class="description__decobox description__decobox--right" />
                      </div>
                    </div>
                </div>

            </div>
        </div>
    </div>
</template>

<script>
import IconDownload from "~/components/Icons/download.vue";
import IconX from "~/components/Icons/x.vue";
import IconFilm from "~/components/Icons/film.vue";
import IconMusic from "~/components/Icons/music-note.vue";
import axios from "axios";

export default {
    components: {
        IconDownload,
        IconX,
        IconFilm,
        IconMusic
    },
        
    props: {
        downloadEntry: {type: Object},
    },

    methods: {
      getQueuedDateString: function(unixTime) {
        const date = new Date(unixTime * 1000);
        const day = ("0" + date.getDay()).slice(-2);
        const month = ("0" + date.getMonth()).slice(-2);
        return day + "." + month + "." + date.getFullYear();
      },
      getDurationString: function(unixTime) {
        const time = new Date(unixTime * 1000);
        const hours = String((time.getHours() - 1));
        const minutes = ("0" + (time.getMinutes())).slice(-2);
        const seconds = ("0" + (time.getSeconds())).slice(-2);
        return ((hours !== "0") ? (hours + ":") : "") + minutes + ":" + seconds; 
      },
      linebreaksToBrTags: function(str) {
        return str.replace("\n", '<br />');
      },
      removeDownload: function() {
        const that = this;
        axios.post("/api", {
          request: "remove_download_entry",
          id: this.downloadEntry.tubio_id
        }).then(function(response) {
          if (response.data.status === "OK") {
            that.$store.dispatch("dlcache/update", that);
          }
        });
      },
    },
}
</script>

<style lang="scss" scoped>
.frame {
  width: 100%;
  border-bottom: 2px solid theme("colors.gray-1");

  & .timestamp {
    font-family: ZillaSlab, serif;
    font-size: 12pt;
    color: theme("colors.text-gray-1");
  }

  & .thumbnail {
    background-image: var(--thumbnail);
    background-size: cover;
    background-position: center;
    background-repeat: no-repeat;
    width: 150px;
    height: calc(150px * (9 / 16));
    position: relative;
    cursor: pointer;
    scrollbar-width: none;

    @media (max-width: theme('screens.md')) {
      width: 100%;
      height: 130px;
    }

    &:hover {
      // transform: scale(1.05); /* shit causes flickering */
    }

    &__vignette {
      position: absolute;
      top: 0;
      bottom: 0;
      right: 0;
      left: 0;
      box-shadow: 0 0 15px 2px #000;
    }

    &__duration {
      position: absolute;
      bottom: 0;
      right: 0;
      padding: 0 3px 3px 0;
      text-align: right;
      font-size: 12pt;
      color: theme("colors.text-gray-1");
      background-color: #000a;
    }
  }

  & .title {
    color: theme("colors.text-gray-1");
    font-size: 22pt;
    max-height: 1.3em;
    overflow-y: hidden;
    overflow-x: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
  }

  & .description {
    color: theme("colors.text-gray-1");
    font-size: 14pt;
    height: 100px;
    overflow-x: hidden;
    overflow-y: scroll;
    position: relative;
    background-color: #0003;

    @media (max-width: theme('screens.md')) {
      overflow: scroll;
    }

    &__decobox {
      position: absolute;

      &--left {
        top: 0;
        left: 0;
        width: 40px;
        height: 40px;
        border-top: 2px solid theme("colors.gray-1");
        border-left: 2px solid theme("colors.gray-1");
        pointer-events: none;
      }

      &--right {
        bottom: 0;
        right: 0;
        width: 40px;
        height: 40px;
        border-bottom: 2px solid theme("colors.gray-1");
        border-right: 2px solid theme("colors.gray-1");
        pointer-events: none;
      }
    }
  }

  & .status {
    &--progressbar {
      background-color: #900;
      height: 5px;

      &__good {
        background-color: #0b0;
        width: var(--download-progress); // Download progress
        transition: width 1s;
      }

      &__text {
        font-size: 24pt;
        color: theme("colors.text-gray-1");
      }
    }

    &--ready {
      height: 45px;
      background-color: theme("colors.text-gray-1");
      transition:
        background-color 0.2s,
        border 0.2s;

      &:hover {
        background-color: theme("colors.purple-1");
        color: #fff;
      }

      & svg {
        height: 35px;
      }
    }

    &--queued {
      font-family: ZillaSlab, serif;
      font-size: 24pt;
      color: theme("colors.text-gray-1");
    }

    &--failed {
      font-family: ZillaSlab, serif;
      font-size: 24pt;
      color: theme("colors.text-error-1");
    }
  }

  & .button-remove {
    width: 35px;
    height: 35px;
    fill: theme("colors.gray-0");
    stroke: theme("colors.gray-0");
    cursor: pointer;
    transition:
      background-color 0.2s,
      border-radius 0.2s;

    &:hover {
      background-color: #d40;
      border-radius: 50%;
    }
  }

  &::-webkit-scrollbar,
  & *::-webkit-scrollbar {
    display: none;
  }

  & .icon--mode {
    width: 20px;
  }
}
</style>
