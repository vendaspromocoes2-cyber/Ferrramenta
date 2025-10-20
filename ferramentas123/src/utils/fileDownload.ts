/**
 * Utility functions for file downloads
 */

/**
 * Downloads a file from a URL
 * @param url - The URL of the file to download
 * @param filename - Optional custom filename
 */
export const downloadFileFromUrl = (url: string, filename?: string) => {
  const link = document.createElement('a');
  link.href = url;
  link.download = filename || url.split('/').pop() || 'download';
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
};

/**
 * Downloads a file from blob data
 * @param blob - Blob data
 * @param filename - Name for the downloaded file
 */
export const downloadFileFromBlob = (blob: Blob, filename: string) => {
  const url = URL.createObjectURL(blob);
  const link = document.createElement('a');
  link.href = url;
  link.download = filename;
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
  URL.revokeObjectURL(url);
};

/**
 * Downloads text content as a file
 * @param content - Text content to download
 * @param filename - Name for the downloaded file
 * @param mimeType - MIME type (default: text/plain)
 */
export const downloadTextFile = (content: string, filename: string, mimeType = 'text/plain') => {
  const blob = new Blob([content], { type: mimeType });
  downloadFileFromBlob(blob, filename);
};

/**
 * Downloads JSON data as a file
 * @param data - JavaScript object to download as JSON
 * @param filename - Name for the downloaded file
 */
export const downloadJsonFile = (data: any, filename: string) => {
  const jsonString = JSON.stringify(data, null, 2);
  downloadTextFile(jsonString, filename, 'application/json');
};

/**
 * Downloads CSV data as a file
 * @param rows - Array of arrays representing CSV rows
 * @param filename - Name for the downloaded file
 */
export const downloadCsvFile = (rows: string[][], filename: string) => {
  const csvContent = rows.map(row => row.join(',')).join('\n');
  downloadTextFile(csvContent, filename, 'text/csv');
};

/**
 * Downloads file from API response
 * @param response - Fetch API response
 * @param filename - Optional custom filename
 */
export const downloadFileFromResponse = async (response: Response, filename?: string) => {
  const blob = await response.blob();
  const contentDisposition = response.headers.get('content-disposition');
  let finalFilename = filename;

  if (!finalFilename && contentDisposition) {
    const filenameMatch = contentDisposition.match(/filename="?(.+)"?/i);
    if (filenameMatch) {
      finalFilename = filenameMatch[1];
    }
  }

  downloadFileFromBlob(blob, finalFilename || 'download');
};
